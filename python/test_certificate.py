import nofl
c = nofl.Certificate('test')

print 'Methods:', dir(c)
print
print 'Name:', c.get_name()
print 'Version:', c.get_version()
print 'Customer Name:', c.get_customer_name()
print 'Product Name:', c.get_product_name()
print 'Product Version:', c.get_product_version()
print 'Product Min Version:', c.get_product_min_version()
print 'Product Max Version:', c.get_product_max_version()
print 'Expire Initial Date:', c.get_initial_date()
print 'Expire Final Date:', c.get_expire_date()
print 'Expire Grace:', c.get_expire_grace()

print '*** Setting customer name'
c.set_customer_name('JohnTheCustomer')
print 'Customer Name:', c.get_customer_name()

