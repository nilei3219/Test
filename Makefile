SRC:=$(wildcard ./*.c)
BIN:=$(patsubst %.c, %, $(SRC))


all:$(BIN)


%:%.c
	gcc $< -L/usr/lib64/mysql -lmysqlclient  -o $@

.PHONY:clean
clean:
	rm -rf $(BIN)


