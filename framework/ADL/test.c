#include <stdio.h>
#include <stdlib.h>
#include "connection.h"

int main( void ) {
	MYSQL *my = NULL;
	connection(&my, "root", "123456", "huojian");
	char *sql = "insert into test values(1, 'abc')";
	update(my, sql);
}
