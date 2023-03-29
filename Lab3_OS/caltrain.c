#include <pthread.h>
#include "caltrain.h"

// Initialize the station.
void station_init(struct station *station)
{
	pthread_cond_init(&station->takeOff, NULL);
	pthread_cond_init(&station->trainArrived, NULL);
	pthread_mutex_init(&station->mutex, NULL);

	station->numberOfPeople = 0; // Number of passengers waiting at the station
	station->enoughSeats = 0;	 // Number of available seats on the train
	station->seats = 0;			 // Number of total seats on the train
}

// Load the train with available seats.
void station_load_train(struct station *station, int count)
{
	station->seats = count; // Set the number of total seats on the train
	pthread_mutex_lock(&(station->mutex));
	station->enoughSeats = count; // Set the number of available seats

	// If there are seats and passengers waiting, broadcast the train's arrival
	if (count != 0 && station->numberOfPeople != 0)
	{
		pthread_cond_broadcast(&(station->trainArrived));
		pthread_cond_wait(&(station->takeOff), &(station->mutex));
	}
	pthread_mutex_unlock(&(station->mutex));
}

// Passenger waits for the train to arrive.
void station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->mutex));
	station->numberOfPeople++; // Increment the number of passengers waiting
	do
	{
		pthread_cond_wait(&(station->trainArrived), &(station->mutex));
		station->enoughSeats--;			// Decrement the number of available seats
	} while (station->enoughSeats < 0); // Keep waiting if there are no available seats
	pthread_mutex_unlock(&(station->mutex));
}

// Passenger boards the train.
void station_on_board(struct station *station)
{
	pthread_mutex_lock(&(station->mutex));

	// If there are passengers waiting and seats available, decrement both counts
	if (station->numberOfPeople != 0 && station->seats != 0)
	{
		station->numberOfPeople--;
		station->seats--;
	}
	pthread_mutex_unlock(&(station->mutex));

	// If there are no passengers waiting or no seats available, signal the train to take off
	if (station->numberOfPeople == 0 || station->seats == 0)
	{
		pthread_cond_signal(&(station->takeOff));
	}
}