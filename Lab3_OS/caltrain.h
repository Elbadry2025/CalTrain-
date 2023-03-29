#include <pthread.h>

struct station {
	pthread_cond_t takeOff;
	pthread_cond_t trainArrived;
	pthread_mutex_t mutex;
	int numberOfPeople;
	int seats;
	int enoughSeats;

};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);