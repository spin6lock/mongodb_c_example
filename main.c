#include <stdio.h>
#include "mongo.h"

void insert_bson_object(mongo *conn) {
	const char ns[] = "test.my_c_library";
	bson b[1];
	bson_init(b);
	bson_append_string(b, "name", "Joe");
	bson_append_int(b, "age", 33);
	bson_finish(b);

	int result = mongo_insert(conn, ns, b, NULL);
	bson_destroy(b);
}

int main() {
	mongo conn[1];
	int status = mongo_client(conn, "127.0.0.1", 27017);

	if (status != MONGO_OK) {
		switch(conn->err) {
			case MONGO_CONN_NO_SOCKET: printf("no socket\n"); return 1;
			case MONGO_CONN_FAIL: printf("connection failed\n"); return 1;
			case MONGO_CONN_NOT_MASTER: printf("not master\n"); return 1;
		}
	}
	printf("connected\n");
	insert_bson_object(conn);
	mongo_destroy(conn);
	printf("disconnect\n");
	return 0;
}
