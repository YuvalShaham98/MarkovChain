tweets: tweets_generator.o markov_chain.o linked_list.o
	gcc tweets_generator.o markov_chain.o linked_list.o -o tweets_generator
snake: snakes_and_ladders.o markov_chain.o linked_list.o
	gcc snakes_and_ladders.o markov_chain.o linked_list.o -o snakes_and_ladders

linked_list.o: linked_list.h linked_list.c
	gcc -c linked_list.c
markov_chain.o: markov_chain.h markov_chain.c linked_list.h
	gcc -c markov_chain.c
tweets_generator.o: tweets_generator.c linked_list.h markov_chain.h
	gcc -c tweets_generator.c
snakes_and_ladders.o: snakes_and_ladders.c linked_list.h markov_chain.h
	gcc -c snakes_and_ladders.c