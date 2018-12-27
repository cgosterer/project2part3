#include "elevator.h"

struct mutex floorLock;

//struct Passenger temppass;

struct Passenger  firstPassList;
struct Passenger  secondPassList;
struct Passenger  thirdPassList;
struct Passenger  fourthPassList;
struct Passenger  fifthPassList;    // declaring lists for each floor
struct Passenger  sixthPassList;
struct Passenger  seventhPassList;
struct Passenger  eigthPassList;
struct Passenger  ninthPassList;
struct Passenger  tenthPassList;

void initfloorlist()
{
	INIT_LIST_HEAD(  &firstPassList.list);
	INIT_LIST_HEAD(  &secondPassList.list);
	INIT_LIST_HEAD(  &thirdPassList.list);
	INIT_LIST_HEAD(  &fourthPassList.list);
	INIT_LIST_HEAD(  &fifthPassList.list);
	INIT_LIST_HEAD(  &sixthPassList.list);    // intitializes each list to empty
	INIT_LIST_HEAD(  &seventhPassList.list);
	INIT_LIST_HEAD(  &eigthPassList.list);
	INIT_LIST_HEAD(  &ninthPassList.list);
	INIT_LIST_HEAD(  &tenthPassList.list);
}

void floorInitLocks ()
{	mutex_init(&floorLock);	        }

void floorCleanLocks ()
{	mutex_destroy(&floorLock);	}


void clearfloorlist()				// this will clear the list of waiting passengers at end of elevator execution
{
	struct list_head *temp;
	struct list_head *dummy;
	struct Passenger * item;

	list_for_each_safe(temp, dummy, &firstPassList.list){
		item = list_entry(temp, struct Passenger, list);
		list_del(temp);
		kfree(item);
	}

	list_for_each_safe(temp, dummy, &secondPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &thirdPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &fourthPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &fifthPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &sixthPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &seventhPassList.list){
       		item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &eigthPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &ninthPassList.list){
        	item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }

	list_for_each_safe(temp, dummy, &tenthPassList.list){
       		item = list_entry(temp, struct Passenger, list);
        	list_del(temp);
        	kfree(item);
        }
}

int issue_request(int passType, int initialFloor, int destinationFloor){		// if error checking needed return some int
	struct Passenger* passPtr = NULL;							// -1 for error, 0 for success

	if (passType == 0){
		passPtr = kmalloc(sizeof(struct Passenger), GFP_KERNEL);
		passPtr->weight = 2;
		passPtr->destination = destinationFloor;
		passPtr->passengers = 1;
		//INIT_LIST_HEAD(&passPtr->list);       //if something breaks, uncomment and see results
	}
	if (passType == 1){
		passPtr = kmalloc(sizeof(struct Passenger), GFP_KERNEL);
		passPtr->weight = 1;
		passPtr->destination = destinationFloor;
		passPtr->passengers = 1;
		//INIT_LIST_HEAD(&passPtr->list);
	}
	if (passType == 2){
		passPtr = kmalloc(sizeof(struct Passenger), GFP_KERNEL);
		passPtr->weight = 4;
		passPtr->destination = destinationFloor;
		passPtr->passengers = 1;
		//INIT_LIST_HEAD(&passPtr->list);
	}
	if (passType == 3){
		passPtr = kmalloc(sizeof(struct Passenger), GFP_KERNEL);
		passPtr->weight = 4;
		passPtr->destination = destinationFloor;
		passPtr->passengers = 2;
		//INIT_LIST_HEAD(&passPtr->list);
	}
	if (initialFloor == 1){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &firstPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 2){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &secondPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 3){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &thirdPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 4){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &fourthPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 5){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &fifthPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 6){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &sixthPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 7){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &seventhPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 8){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &eigthPassList.list);
		mutex_unlock(&floorLock);
	}
	if (initialFloor == 9){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &ninthPassList.list);
		 mutex_unlock(&floorLock);
	}
	if (initialFloor == 10){
		mutex_lock(&floorLock);
		list_add_tail(&passPtr->list, &tenthPassList.list);
		mutex_unlock(&floorLock);
	}

	if(initialFloor < 1 || initialFloor > 10)
		return 1;

	if( passType < 0 || passType > 3)
                return 1;

	if( destinationFloor < 1 || destinationFloor > 10)
                return 1;

	return 0;
}

int numPassengers(int floor){
	struct list_head *temp;
	struct list_head *dummy;
	struct Passenger *item;
	int numPassengers = 0;
	if (floor == 1){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &firstPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}

	if (floor == 2){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &secondPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct  Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 3){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &thirdPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 4){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fourthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 5){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fifthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 6){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &sixthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 7){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &seventhPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 8){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &eigthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 9){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &ninthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}
	if (floor == 10){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &tenthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			numPassengers++;
			if(item->passengers == 2)
				numPassengers++;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return numPassengers;
	}

	else
		return -1;

}

 int getWaitingLoad(int floor){
	 struct list_head *temp;
	 struct list_head *dummy;
	 struct Passenger *item;
	 int weightOfLoad = 0;
	 if (floor == 1){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &firstPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 2){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &secondPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 3){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &thirdPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 4){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fourthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 5){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fifthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 6){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &sixthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 7){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &seventhPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 8){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &eigthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 9){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &ninthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}
	if (floor == 10){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &tenthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			weightOfLoad += item->weight;
		//can access item->num
		}
		mutex_unlock(&floorLock);
		return weightOfLoad;
	}

	else
		return -1;

 }
 struct Passenger* checkWaiter(int floor){
	 struct list_head *temp;
	 struct list_head *dummy;
	 struct Passenger *item;
	 if (floor == 1){
		mutex_lock(&floorLock);
		/* Use this if you need to change pointers */
		list_for_each_safe(temp, dummy, &firstPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 2){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &secondPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 3){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &thirdPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 4){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fourthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 5){
		mutex_lock(&floorLock);
		/* Use this if you need to change pointers */
		list_for_each_safe(temp, dummy, &fifthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 6){
		mutex_lock(&floorLock);
		/* Use this if you need to change pointers */
		list_for_each_safe(temp, dummy, &sixthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 7){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &seventhPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 8){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &eigthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 9){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &ninthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 10){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &tenthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}

		item = NULL;
		mutex_unlock(&floorLock);
		return item;
 }
 struct Passenger* loadWaiter(int floor){
	 struct list_head *temp;
	 struct list_head *dummy;
	 struct Passenger *item;
	//struct Passenger * tempptr;			// for returning the temp struct, may need to be gobal for return
	//tempptr = kmalloc(sizeof(struct Passenger), GFP_KERNEL);

	 if (floor == 1){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &firstPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);

			//temppass->weight = item->weight;
			//temppass->passengers = item->passengers;
			//temppass->destination = item->destination;
			//temppass->list = item->list;
			//tempptr = &temppass;
			//list_del(temp);
			//return tempptr;
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 2){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &secondPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp,  struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 3){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &thirdPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 4){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fourthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 5){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &fifthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 6){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &sixthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 7){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &seventhPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 8){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &eigthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 9){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &ninthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}
	if (floor == 10){
		/* Use this if you need to change pointers */
		mutex_lock(&floorLock);
		list_for_each_safe(temp, dummy, &tenthPassList.list) {
		/* Use this to get the surrounding struct */
			item = list_entry(temp, struct Passenger, list);
			list_del(temp);
			mutex_unlock(&floorLock);
			return item;
		//can access item->num
		}
	}

		item = NULL;
		return item;
 }
