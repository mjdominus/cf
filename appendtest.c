
#include "cf_per.h"
#include "cf_append.h"

int main(void) 
{
  {
    CF cf123 = new_array(1, 2, 3, 0);
    CF cf1234 = append1(cf123, 4);
    CF cf12345 = append1(cf1234, 5);
    print_cf(cf12345);
  }
  {
    CF cf123 = new_array(1, 2, 3, 0);
    CF cf1234 = new_array(1, 2, 3, 4, 0);
    CF cf1234123 = appendcf(cf1234, cf123, (CF)0);
    print_cf(cf1234123);
  }
  {
    CF cf123a = new_array(1, 2, 3, 0);
    CF cf123b = new_array(1, 2, 3, 0);
    CF cf1234 = new_array(1, 2, 3, 4, 0);
    CF cf1231234123 = appendcf(cf123a, cf1234, cf123b, (CF)0);
    print_cf(cf1231234123);
  }
}
