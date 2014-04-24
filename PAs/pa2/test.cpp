#include <stdlib.h>
#include <iostream>


using namespace std;

int main(){

	cout<< "size of int	 :" << sizeof(int) << endl;
	cout<< "size of uint	 :" << sizeof(unsigned int) << endl;
	cout<< "size of double	 :" << sizeof(double) << endl;
	cout<< "size of long	 :" << sizeof(long) << endl;
	cout<< "size of long long:" << sizeof(long long) << endl;
	cout<< "size of int64_t	 :" << sizeof(int64_t) << endl;
	
	char tmp_str[] = "34155013283297";
	int64_t a = strtoull(tmp_str,NULL,10);
	int64_t b = atoll(tmp_str);
	cout << "so far so good" << endl;
	cout << a << endl;
	cout << b << endl;
	return 1;

}
