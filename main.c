#include <stdio.h>
#include "mongo.h"

const char ns[] = "test.my_c_library";

void insert_bson_object(mongo *conn) {
	bson b[1];
	bson_init(b);
	bson_append_string(b, "name", "Joe");
	bson_append_int(b, "age", 33);
	bson_finish(b);

	int result = mongo_insert(conn, ns, b, NULL);
	bson_destroy(b);
}

void query_all_bson_object(mongo *conn) {
	mongo_cursor cursor[1];
	mongo_cursor_init(cursor, conn, ns);

	while (mongo_cursor_next(cursor) == MONGO_OK) {
		bson_print(&cursor->current);
	}
	mongo_cursor_destroy(cursor);
}

void update_bson_object(mongo *conn) {
	//query: {"name":"Joe"}
	bson cond[1];
	bson_init(cond);
	bson_append_string(cond, "name", "Joe");
	bson_finish(cond);

	//op: {"$inc":{"age":2"}}
	bson op[1];
	int ret;
	bson_init(op);
	bson_append_start_object(op, "$inc");
	bson_append_int(op, "age", 2);
	bson_append_finish_object(op);
	if ((ret = bson_finish(op)) != BSON_OK) {
		printf("bson error: %d", ret);
	} else {
		mongo_update(conn, ns, cond, op, 0, NULL);
	}
}

void delete_bson_object(mongo *conn) {
	// query: {"name":"Joe"}
	bson b[1];
	bson_init(b);
	bson_append_string(b, "name", "Joe");
	bson_finish(b);
	mongo_remove(conn, ns, b, NULL);
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

	//Create
	insert_bson_object(conn);
	printf("after insert\n");
	//Query
	query_all_bson_object(conn);

	//Update: Joe's age add 2
	update_bson_object(conn);	
	printf("after update\n");
	query_all_bson_object(conn);

	//Delete
	delete_bson_object(conn);
	printf("after delete\n");
	query_all_bson_object(conn);
	mongo_destroy(conn);
	printf("disconnect\n");
	return 0;
}
