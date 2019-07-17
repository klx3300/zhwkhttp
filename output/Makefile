__fake_main__: server

zhwkre/bubble_sort.o: ../zhwkre/algorithm/bubble_sort.c
	@echo "   gcc   zhwkre/bubble_sort.o"
	@gcc -c  -o zhwkre/bubble_sort.o ../zhwkre/algorithm/bubble_sort.c
zhwkre/bss.o: ../zhwkre/bss/bss.c
	@echo "   gcc   zhwkre/bss.o"
	@gcc -c  -o zhwkre/bss.o ../zhwkre/bss/bss.c
zhwkre/mutex.o: ../zhwkre/concurrent/mutex.c
	@echo "   gcc   zhwkre/mutex.o"
	@gcc -c  -o zhwkre/mutex.o ../zhwkre/concurrent/mutex.c
zhwkre/threading.o: ../zhwkre/concurrent/threading.c
	@echo "   gcc   zhwkre/threading.o"
	@gcc -c  -o zhwkre/threading.o ../zhwkre/concurrent/threading.c
zhwkre/list.o: ../zhwkre/containers/list.c
	@echo "   gcc   zhwkre/list.o"
	@gcc -c  -o zhwkre/list.o ../zhwkre/containers/list.c
zhwkre/unordered_map.o: ../zhwkre/containers/unordered_map.c
	@echo "   gcc   zhwkre/unordered_map.o"
	@gcc -c  -o zhwkre/unordered_map.o ../zhwkre/containers/unordered_map.c
zhwkre/vector.o: ../zhwkre/containers/vector.c
	@echo "   gcc   zhwkre/vector.o"
	@gcc -c  -o zhwkre/vector.o ../zhwkre/containers/vector.c
zhwkre/btree.o: ../zhwkre/containers/btree.c
	@echo "   gcc   zhwkre/btree.o"
	@gcc -c -g -o zhwkre/btree.o ../zhwkre/containers/btree.c
zhwkre/avltree.o: ../zhwkre/containers/AVLTree.c
	@echo "   gcc   zhwkre/avltree.o"
	@gcc -c -g -o zhwkre/avltree.o ../zhwkre/containers/AVLTree.c
zhwkre/set.o: ../zhwkre/containers/set.c
	@echo "   gcc   zhwkre/set.o"
	@gcc -c -g -o zhwkre/set.o ../zhwkre/containers/set.c
zhwkre/socket.o: ../zhwkre/network/socket.c
	@echo "   gcc   zhwkre/socket.o"
	@gcc -c  -o zhwkre/socket.o ../zhwkre/network/socket.c
zhwkre/tcp.o: ../zhwkre/network/tcp.c
	@echo "   gcc   zhwkre/tcp.o"
	@gcc -c  -o zhwkre/tcp.o ../zhwkre/network/tcp.c
zhwkre/udp.o: ../zhwkre/network/udp.c
	@echo "   gcc   zhwkre/udp.o"
	@gcc -c  -o zhwkre/udp.o ../zhwkre/network/udp.c
zhwkre/serialization.o: ../zhwkre/serialization/serialization.c
	@echo "   gcc   zhwkre/serialization.o"
	@gcc -c  -o zhwkre/serialization.o ../zhwkre/serialization/serialization.c
zhwkre/utils.o: ../zhwkre/utils/utils.c
	@echo "   gcc   zhwkre/utils.o"
	@gcc -c  -o zhwkre/utils.o ../zhwkre/utils/utils.c
zhwkre/fmt.o: ../zhwkre/format/format.c
	@echo "   gcc   zhwkre/fmt.o"
	@gcc -c  -o zhwkre/fmt.o ../zhwkre/format/format.c
zhwkre/btree_test.o: ../zhwkre/tests/btree_test.c
	@echo "   gcc   zhwkre/btree_test.o"
	@gcc -c -g -o zhwkre/btree_test.o ../zhwkre/tests/btree_test.c
zhwkre/bss_test.o: ../zhwkre/tests/bss_test.c
	@echo "   gcc   zhwkre/bss_test.o"
	@gcc -c -g -o zhwkre/bss_test.o ../zhwkre/tests/bss_test.c
zhwkre/avltree_test.o: ../zhwkre/tests/avltree_test.c
	@echo "   gcc   zhwkre/avltree_test.o"
	@gcc -c -g -o zhwkre/avltree_test.o ../zhwkre/tests/avltree_test.c
zhwkre/vector_test.o: ../zhwkre/tests/vector_test.c
	@echo "   gcc   zhwkre/vector_test.o"
	@gcc -c -g -o zhwkre/vector_test.o ../zhwkre/tests/vector_test.c
zhwkre/btree_test: zhwkre/btree_test.o zhwkre/btree.o zhwkre/fmt.o 
	@echo "   LD   zhwkre/btree_test"
	@gcc  -o zhwkre/btree_test zhwkre/btree_test.o zhwkre/btree.o zhwkre/fmt.o 
zhwkre/bss_test: zhwkre/bss_test.o zhwkre/bss.o zhwkre/fmt.o 
	@echo "   LD   zhwkre/bss_test"
	@gcc  -o zhwkre/bss_test zhwkre/bss_test.o zhwkre/bss.o zhwkre/fmt.o 
zhwkre/avltree_test: zhwkre/avltree_test.o zhwkre/avltree.o zhwkre/fmt.o 
	@echo "   LD   zhwkre/avltree_test"
	@gcc  -o zhwkre/avltree_test zhwkre/avltree_test.o zhwkre/avltree.o zhwkre/fmt.o 
zhwkre/vector_test: zhwkre/vector_test.o zhwkre/vector.o zhwkre/fmt.o 
	@echo "   LD   zhwkre/vector_test"
	@gcc  -o zhwkre/vector_test zhwkre/vector_test.o zhwkre/vector.o zhwkre/fmt.o 
parser.o: ../parser/parser.cc ../parser/parser.h
	@echo "   g++   parser.o"
	@g++ -c -g -std=gnu++14 -o parser.o ../parser/parser.cc
parsertest.o: ../parser/testparser.cc
	@echo "   g++   parsertest.o"
	@g++ -c -g -std=gnu++14 -o parsertest.o ../parser/testparser.cc
config.o: ../config/config.cc ../config/config.h
	@echo "   g++   config.o"
	@g++ -c -g -std=gnu++14 -o config.o ../config/config.cc
stroper.o: ../stroper/stroper.cc ../stroper/stroper.h
	@echo "   g++   stroper.o"
	@g++ -c -g -std=gnu++14 -o stroper.o ../stroper/stroper.cc
task.o: ../task/task.cc ../task/task.h
	@echo "   g++   task.o"
	@g++ -c -g -std=gnu++14 -o task.o ../task/task.cc
main.o: ../main.cc
	@echo "   g++   main.o"
	@g++ -c -g -std=gnu++14 -o main.o ../main.cc
parsertest: parsertest.o parser.o zhwkre/fmt.o zhwkre/bss.o 
	@echo "   LD   parsertest"
	@g++ -lpthread -o parsertest parsertest.o parser.o zhwkre/fmt.o zhwkre/bss.o 
server: main.o task.o stroper.o config.o parser.o zhwkre/fmt.o zhwkre/bss.o zhwkre/tcp.o zhwkre/socket.o zhwkre/utils.o 
	@echo "   LD   server"
	@g++ -lpthread -o server main.o task.o stroper.o config.o parser.o zhwkre/fmt.o zhwkre/bss.o zhwkre/tcp.o zhwkre/socket.o zhwkre/utils.o 
clean:
	@echo "  CLEAN   all files."
	-@rm zhwkre/bubble_sort.o zhwkre/bss.o zhwkre/mutex.o zhwkre/threading.o zhwkre/list.o zhwkre/unordered_map.o zhwkre/vector.o zhwkre/btree.o zhwkre/avltree.o zhwkre/set.o zhwkre/socket.o zhwkre/tcp.o zhwkre/udp.o zhwkre/serialization.o zhwkre/utils.o zhwkre/fmt.o zhwkre/btree_test.o zhwkre/bss_test.o zhwkre/avltree_test.o zhwkre/vector_test.o zhwkre/btree_test zhwkre/bss_test zhwkre/avltree_test zhwkre/vector_test parser.o parsertest.o config.o stroper.o task.o main.o parsertest server  2>/dev/null >/dev/null || true
