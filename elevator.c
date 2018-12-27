#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include "elevator.h"

//Variables needed to be stored by the elevator for its scheduling and regular running
int servicedCount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int elevatorCurrentFloor = 1, elevatorDestination = 1;
const int elevatorMaxWeight = 20, elevatorMaxCapacity = 8;
struct Passenger * elevatorOccupants[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int elevatorNumOccupants = 0;
enum ElevatorState elevatorCurrentState = OFFLINE;
enum ElevatorState elevatorDirection = IDLE;

//Stores the kthread running the elevator when active
struct task_struct *runningElevator;

//Mutex to use as lock
struct mutex elevatorLock, threadLock;

//Function prototpyes
int shouldLoad(int);
struct Passenger * loadPassenger(int);
int shouldUnload(int);
void unloadPassenger(int);
void goUp(void);
void goDown(void);
enum ElevatorState getRelativeDirection(int floor);
int normalRunning(void* data);
int exitRunning(void);
  
void elevatorInitLocks() {
  mutex_init(&elevatorLock);
  mutex_init(&threadLock);
}

void elevatorCleanLocks() {
  mutex_destroy(&elevatorLock);
  mutex_destroy(&threadLock);
}

enum ElevatorState getState()
{
  enum ElevatorState x;
  mutex_lock(&elevatorLock);
  x = elevatorCurrentState;
  mutex_unlock(&elevatorLock);
  return x;
}

int getFloor() {
  int x;
  mutex_lock(&elevatorLock);
  x = elevatorCurrentFloor;
  mutex_unlock(&elevatorLock);
  return x;
}

int getDest() {
  int x;
  mutex_lock(&elevatorLock);
  x = elevatorDestination;
  mutex_unlock(&elevatorLock);
  return x;
}

int getWeight() {
  int totalWeight = 0;
  int i;
  mutex_lock(&elevatorLock);
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      totalWeight += elevatorOccupants[i][0].weight;
    }
  mutex_unlock(&elevatorLock);
  return totalWeight;
}

int getCapacity() {
  int totalPeople = 0;
  int i;
  mutex_lock(&elevatorLock);
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      totalPeople += elevatorOccupants[i]->passengers;
    }
  mutex_unlock(&elevatorLock);
  return totalPeople;
}

int serviced(int floor) {
  int x;
  if(floor > 9 || floor < 0)
    return -1;
  else
    {
      mutex_lock(&elevatorLock);
      x = servicedCount[floor];
      mutex_unlock(&elevatorLock);
      return x;
    }
}

int shouldLoad(int floor) {
  int weight = 0, capacity = 0;
  int i;
  struct Passenger * waiting;
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      weight += elevatorOccupants[i]->weight;
      capacity += elevatorOccupants[i]->passengers;
    }
  waiting = checkWaiter(floor);
  if(waiting == NULL)
    return 0;
  if(floor == 10) {printk(KERN_ALERT "Saw him\n");
    printk(KERN_ALERT "Elevator Direction %d\n", elevatorDirection);
    printk(KERN_ALERT "Elevator State %d\n", elevatorCurrentState);
    printk(KERN_ALERT "His Direction %d\n", getRelativeDirection(waiting->destination));
  }
  if(elevatorCurrentState != IDLE && getRelativeDirection(waiting->destination) != elevatorDirection)
    return 0;
  if(floor == 10) printk(KERN_ALERT "Checked his weight\n");
  weight += waiting->weight;
  capacity += waiting->passengers;
  if(weight > elevatorMaxWeight || capacity > elevatorMaxCapacity)
    return 0;
  else
    {
      if(floor == 10) printk(KERN_ALERT "Returning True, we should get him\n");
      return 1;
    }
}

struct Passenger * loadPassenger(int floor) {
  printk(KERN_ALERT "The elevator is adding a passenger on floor %d\n", floor);
  elevatorOccupants[elevatorNumOccupants] = loadWaiter(floor);
  elevatorNumOccupants++;
  servicedCount[floor - 1]++;
  return elevatorOccupants[elevatorNumOccupants - 1];
}

void loadMany(int floor) {
  struct Passenger * justLoaded;
  while(shouldLoad(floor))
    {
      justLoaded = loadPassenger(floor);
      if(elevatorDirection == UP && justLoaded->destination > elevatorDestination)
	elevatorDestination = justLoaded->destination;
      if(elevatorDirection == DOWN && justLoaded->destination < elevatorDestination)
	elevatorDestination = justLoaded->destination;
    }
}

int shouldUnload(int floor) {
  int i;
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      if(elevatorOccupants[i]->destination == floor)
	return 1;
    }
  return 0;
}

void unloadPassenger(int floor) {
  struct Passenger *temp;
  int numUnloaded = 0;
  int i, j;
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      if(elevatorOccupants[i]->destination == floor)
	{
	  numUnloaded++;
	  kfree(elevatorOccupants[i]);
	  elevatorOccupants[i] = NULL;
	}
    }
  i = elevatorNumOccupants - 1;
  j = 0;
  while(i > j)
    {
      while(elevatorOccupants[i] == NULL)
	i--;
      while(elevatorOccupants[j] != NULL)
	j++;
      if(i > j) {
	temp = elevatorOccupants[j];
	elevatorOccupants[j] = elevatorOccupants[i];
	elevatorOccupants[i] = temp;
      }
    }
  elevatorNumOccupants -= numUnloaded;
  printk(KERN_ALERT "The elevator has unloaded %d passengers from floor %d", numUnloaded, floor);
}

void generalLoadingProcedure(int floor) {
  mutex_lock(&elevatorLock);
  if(shouldLoad(floor) || shouldUnload(floor))
    {
      printk(KERN_ALERT "State = LOADING\n");
      elevatorCurrentState = LOADING;
      mutex_unlock(&elevatorLock);
      ssleep(1);
      mutex_lock(&elevatorLock);
      unloadPassenger(floor);
      loadMany(floor);
    }
  mutex_unlock(&elevatorLock);
}

void goUp() {
  ssleep(2);
  mutex_lock(&elevatorLock);
  elevatorCurrentFloor++;
  printk(KERN_ALERT "The elevator has gone up to floor %d\n", elevatorCurrentFloor);
  mutex_unlock(&elevatorLock);
}

void goDown() {
  ssleep(2);
  mutex_lock(&elevatorLock);
  elevatorCurrentFloor--;
  printk(KERN_ALERT "The elevator has gone down to floor %d\n", elevatorCurrentFloor);
  mutex_unlock(&elevatorLock);
}

enum ElevatorState getRelativeDirection(int floor) {
  if(floor > elevatorCurrentFloor)
    return UP;
  else
    return DOWN;
}

int normalRunning(void* data) {
  int i;
  //Note: When we're at the top of this loop, current state shouldn't be loading or offline
  while(!kthread_should_stop())
    {
      //If we're idle, check whether anybody is waiting to be picked up
      //Note, we should only be idle if we're currently empty
      mutex_lock(&elevatorLock);
      if(elevatorCurrentState == IDLE)
	{
	  for(i = 1; i <= 10; i++)
	    {
	      if(checkWaiter(i) != NULL)
		{
		  elevatorDestination = i;
		  if(elevatorCurrentFloor > i){
		    elevatorDirection = DOWN;
		    elevatorCurrentState = DOWN;
		    break;
		  }
		  else if(elevatorCurrentFloor < i){
		    elevatorDirection = UP;
		    elevatorCurrentState = UP;
		    break;
		  }
		  else {
		    elevatorCurrentState = LOADING;
		    loadPassenger(i);
		    elevatorDestination = elevatorOccupants[0]->destination;
		    mutex_unlock(&elevatorLock);
		    ssleep(1);
		    mutex_lock(&elevatorLock);
		    elevatorDirection = getRelativeDirection(elevatorDestination);
		    loadMany(elevatorCurrentFloor);
		    elevatorCurrentState = elevatorDirection;
		    break;
		  }
		}
	    }
	}
      //If we're not IDLE, move towards our destination
      else
	{
	  //Move, then check whether the current destination should be extended further up/down
	  if(elevatorCurrentState == UP)
	    {
	      mutex_unlock(&elevatorLock);
	      goUp();
	      mutex_lock(&elevatorLock);
	      for(i = 10; i > elevatorDestination; i--)
		if(checkWaiter(i) != NULL)
		  elevatorDestination = i;
	    }
	  else
	    {
	      mutex_unlock(&elevatorLock);
	      goDown();
	      mutex_lock(&elevatorLock);
	      for(i = 1; i < elevatorDestination; i++)
		if(checkWaiter(i) != NULL)
		  elevatorDestination = i;
	    }
	  mutex_unlock(&elevatorLock);
	  generalLoadingProcedure(elevatorCurrentFloor);
	  mutex_lock(&elevatorLock);
	  elevatorCurrentState = elevatorDirection;
	  if(elevatorCurrentFloor == elevatorDestination)
	    {
	      printk(KERN_ALERT "At our destination with %d occupants\n", elevatorNumOccupants);
	      printk(KERN_ALERT "Our direction is %d\n", elevatorCurrentState);
	      if(elevatorNumOccupants == 0)
		elevatorCurrentState = IDLE;
	      else if(elevatorCurrentState == UP)
		{
		  elevatorCurrentState = elevatorDirection = DOWN;
		}
	      else
		{
		  elevatorCurrentState = elevatorDirection = UP;
		}
	      printk(KERN_ALERT "Our direction is now%d\n", elevatorCurrentState);
	    }
	  mutex_unlock(&elevatorLock);
	  generalLoadingProcedure(elevatorCurrentFloor);
	  mutex_lock(&elevatorLock);
	}
      mutex_unlock(&elevatorLock);
      //Give up the CPU to other threads so if we're simply idling with no waiting passengers we don't busy wait
      ssleep(0);
    }
  return exitRunning();
}

//Strategy for emptying elevator efficiently:
//Go either up or down in the direction we need to go the least far
//Then go the other direction
int exitRunning() {
  int topFloor = 0, botFloor = 999;
  int upDist, downDist;
  int i;
  //Figure out how high up and down we need to go first
  mutex_lock(&elevatorLock);
  for(i = 0; i < elevatorNumOccupants; i++)
    {
      if(elevatorOccupants[i]->destination > topFloor)
	topFloor = elevatorOccupants[i]->destination;
      if(elevatorOccupants[i]->destination < botFloor)
	botFloor = elevatorOccupants[i]->destination;
    }
  upDist = topFloor - elevatorCurrentFloor;
  downDist = elevatorCurrentFloor - botFloor;
  mutex_unlock(&elevatorLock);
  //If we go up first, go up
  if(upDist <= downDist && upDist >= 0)
    {
      mutex_lock(&elevatorLock);
      while(elevatorCurrentFloor != topFloor)
	{
	  if(shouldUnload(elevatorCurrentFloor))
	    {
	      elevatorCurrentState = LOADING;
	      unloadPassenger(elevatorCurrentFloor);
	      mutex_unlock(&elevatorLock);
	      ssleep(1);
	      mutex_lock(&elevatorLock);
	    }
	  elevatorCurrentState = UP;
	  mutex_unlock(&elevatorLock);
	  goUp();
	  mutex_lock(&elevatorLock);
	}
      mutex_unlock(&elevatorLock);
    }
  //Go down
  if(downDist >= 0)
    {
      mutex_lock(&elevatorLock);
      while(elevatorCurrentFloor != botFloor)
	{
	  if(shouldUnload(elevatorCurrentFloor))
	    {
	      elevatorCurrentState = LOADING;
	      unloadPassenger(elevatorCurrentFloor);
	      mutex_unlock(&elevatorLock);
	      ssleep(1);
	      mutex_lock(&elevatorLock);
	    }
	  elevatorCurrentState = DOWN;
	  mutex_unlock(&elevatorLock);
	  goDown();
	  mutex_lock(&elevatorLock);
	}
      mutex_unlock(&elevatorLock);
    }
  //If we go down first, go up
  if(upDist >= downDist && upDist >= 0)
    {
      mutex_lock(&elevatorLock);
      while(elevatorCurrentFloor != topFloor)
	{
	  if(shouldUnload(elevatorCurrentFloor))
	    {
	      elevatorCurrentState = LOADING;
	      unloadPassenger(elevatorCurrentFloor);
	      mutex_unlock(&elevatorLock);
	      ssleep(1);
	      mutex_lock(&elevatorLock);
	    }
	  elevatorCurrentState = UP;
	  mutex_unlock(&elevatorLock);
	  goUp();
	  mutex_lock(&elevatorLock);
	}
      mutex_unlock(&elevatorLock);
    }
  //Whatever our final floor is, unload here if need be
  mutex_lock(&elevatorLock);
   if(shouldUnload(elevatorCurrentFloor))
     {
       elevatorCurrentState = LOADING;
       unloadPassenger(elevatorCurrentFloor);
       mutex_unlock(&elevatorLock);
       ssleep(1);
       mutex_lock(&elevatorLock);
     }
   elevatorCurrentState = OFFLINE;
   mutex_unlock(&elevatorLock);
   return 0;
}

int startElevator() {
  int i;
  mutex_lock(&threadLock);
  mutex_lock(&elevatorLock);
  if(elevatorCurrentState != OFFLINE)
    {
      mutex_unlock(&elevatorLock);
      mutex_unlock(&threadLock);
      return 1;
    }
  for(i = 0; i < 8; i++)
    {
      servicedCount[i] = 0;
      elevatorOccupants[i] = NULL;
    }
  servicedCount[8] = servicedCount[9] = 0;
  elevatorCurrentFloor = 1;
  elevatorDestination = 1;
  elevatorNumOccupants = 0;
  elevatorCurrentState = IDLE;
  elevatorDirection = IDLE;
  mutex_unlock(&elevatorLock);
  printk(KERN_INFO "Starting Elevator Thread\n");
  runningElevator = kthread_run(normalRunning, NULL, "Elevator Simulation");
  printk(KERN_INFO "Elevator Thread Started\n");
  mutex_unlock(&threadLock);
  return 0;
}

int stopElevator() {
  mutex_lock(&threadLock);
  mutex_lock(&elevatorLock);
  if(elevatorCurrentState == OFFLINE)
    {
      mutex_unlock(&elevatorLock);
      mutex_unlock(&threadLock);
      return 1;
    }
  mutex_unlock(&elevatorLock);
  kthread_stop(runningElevator);
  mutex_unlock(&threadLock);
  return 0;
}
