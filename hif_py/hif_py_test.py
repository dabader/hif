import unittest
import random
import libhif_pybind as hif


class MyPythonAPITest(unittest.TestCase):
  def process_statement(self, stmt, conta, directory):
    self.assertEqual(stmt, directory[conta])
    return conta + 1
  
  
  def process_each_statement(self, reader, directory, num):
    conta = 0
    def process(stmt):
      nonlocal conta
      conta = self.process_statement(stmt, conta, directory)
    reader.each(process)
    self.assertEqual(conta, num)


  def test_trivial(self):
    fname = "hif_test_data1"
    stmt_vector = []
    hif_writer = hif.Hif_write.create(fname, "testtool", "0.2.1")
    self.assertIsNotNone(hif_writer, "Hif_write.create returned None")
    stmt = hif.Hif_write.create_assign()
    stmt.instance = "jojojo"
    stmt.add_input("A", "0");
    stmt.add_input("A", "1");
    stmt.add_input("A", "2");
    stmt.add_input("A", "3");
    stmt.add_output("Z");
    stmt.add_attr("loc", "3");
    hif_writer.add(stmt)
    stmt_vector.append(stmt)
    hif_writer = None #should call deconstructor
    hif_reader = hif.Hif_read.open(fname)
    self.assertIsNotNone(hif_reader, "Hif_read.open returned None")
    self.assertEqual(hif_reader.get_tool(), "testtool")
    self.assertEqual(hif_reader.get_version(), "0.2.1")
    self.process_each_statement(hif_reader, stmt_vector, len(stmt_vector))
    hif_reader = None
    stmt = None
  def test_large_stmt(self):
    fname = "hif_test_data2"
    stmt_vector = []
    hif_writer = hif.Hif_write.create(fname, "testtool", "0.0.3")
    self.assertIsNotNone(hif_writer, "Hif_write.create returned None")
    stmt = hif.Hif_write.create_assign()
    stmt.instance = "jojojo"
    for i in range(1024):
      stmt.add_input(str(i), "a_longer_string_" + str(i))
    for i in range(1024):
      stmt.add_output(str(i) + "_out", "a_longer_string_" + str(i))
    hif_writer.add(stmt)
    stmt_vector.append(stmt)
    hif_writer = None #should call deconstructor
    hif_reader = hif.Hif_read.open(fname)
    self.assertIsNotNone(hif_reader, "Hif_read.open returned None")
    self.process_each_statement(hif_reader, stmt_vector, len(stmt_vector))

  def stmt_class_check(self, stmt_type):
    create_method_name = f"create_{stmt_type}"
    create_method = getattr(hif.Hif_write, create_method_name)
    stmt = create_method()

    is_method_name = f"is_{stmt_type}"
    is_method = getattr(stmt, is_method_name)
    self.assertTrue(is_method(), f"{stmt_type} check failed")
    stmt = None

  def test_statement_class(self):
    fname = "hif_test_statement_class_check"
    self.stmt_class_check("node")
    self.stmt_class_check("assign")
    self.stmt_class_check("attr")
    self.stmt_class_check("open_call")
    self.stmt_class_check("closed_call")
    self.stmt_class_check("open_def")
    self.stmt_class_check("closed_def")
    self.stmt_class_check("end")
    self.stmt_class_check("use")
  
  def test_empty_string(self):
    fname = "hif_empty_string"
    hif_writer = hif.Hif_write.create(fname, "lnast", "test")
    self.assertIsNotNone(hif_writer, "Hif_write.create returned None")
    stmt = hif.Hif_write.create_attr()
    stmt.add_attr("name", "")
    hif_writer.add(stmt)
    hif_writer = None

    hif_reader = hif.Hif_read.open(fname)
    self.assertIsNotNone(hif_reader, "Hif_read.open returned None")
    hif_reader.get_tool()
    hif_reader.get_version()
    hif_reader.next_stmt()
    stmt = hif_reader.get_current_stmt()
    self.assertEqual(stmt.attr[0].lhs, "name")
    self.assertEqual(stmt.attr[0].rhs, "")


   

  def test_strings_match(self):
    fname = "hif_test_strings_match"
    inp_vector = []
    out_vector = []
    hif_writer = hif.Hif_write.create(fname, "testtool", "0.0.4")
    self.assertIsNotNone(hif_writer, "Hif_write.create returned None")
    def get_random_string():
      counter = getattr(get_random_string, "counter", 0)  # Static variable equivalent
    
      str_parts = []
      
      if random.randint(0, 1):
        str_parts.append("$")
      
      str_parts.append(str(counter))
      
      if random.randint(0, 1):
        str_parts.append("something_quite_large_and_not_reusable")
      
      if random.randint(0, 1):
        str_parts.append(str(1020303 + counter))
      
      get_random_string.counter = counter + 1  # Update the static variable
      return ''.join(str_parts)
 
    for i in range(1024):
      stmt = hif.Statement()
      if(i & 1):
        stmt = hif.Hif_write.create_assign()
      else:
        stmt = hif.Hif_write.create_node()

      lhs = get_random_string()
      rhs = get_random_string()
      inp_vector.append(lhs)
      out_vector.append(rhs)
      
      stmt.add_input(lhs, i)
      stmt.add_output(rhs, i)

      hif_writer.add(stmt)
    hif_writer = None
    hif_reader = hif.Hif_read.open(fname)
    self.assertIsNotNone(hif_reader, "Hif_read.open returned None")

    conta = 0
    def process_statement(stmt):
      nonlocal conta

      if conta & 1:
        self.assertTrue(stmt.is_assign())
      else:
        self.assertTrue(stmt.is_node())
      
      self.assertTrue(len(stmt.attr) == 0)
      self.assertEqual(len(stmt.io), 2)

      for io in stmt.io:
        self.assertTrue(io.is_lhs_string())
        self.assertTrue(io.is_rhs_int64())
        v = io.get_rhs_int64()
        self.assertEqual(conta, v)
        
        if io.input:
          self.assertEqual(inp_vector[conta], io.lhs)
        else:
          self.assertEqual(out_vector[conta], io.lhs)

      conta += 1
    hif_reader.each(process_statement)
    self.assertEqual(conta, len(inp_vector))
    self.assertEqual(conta, len(out_vector))

if __name__ == '__main__':
    unittest.main()
