# skip comment
def abc (x):
    print ("in abc()", x)
    # test
    def d (y):
        print ("in d()", y)
        # test
        return x + y

    return d

class range:

    def __init__( self, val ):
        self.val = val
        self.i = 0

    def __next__( self ):
        if self.i < self.val:
            self.i = self.i + 1
            return self.i
        else:
            return 0

a = list (1, 2)
a.append (3)
a.append ('yaho')
a.append (4)
print (a)
print (a.len ())

for i in range(5):
    print( i )


class cls:
    e = 3

    def f( self, a ):
        print (self.e + a)

    def g( self, a ):
        self.e = a

#c = cls()
#c.f (99)
#c.g (7)
#c.f (99)
print ("te\"\t'st")
print ('abc\'"def')
add = abc (2)
print (add(4))

b = [2, 3, '4']
print( b )
print( b[2] )

