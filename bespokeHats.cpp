
/*-------------------------------------------------------------------------*
 *---									---*
 *---		bespokeHats.cpp						---*
 *---									---*
 *---									---*
 *---	    This file defines a program that showcases multi-threaded	---*
 *---	programming by simulates the manufacture and sale of fancy	---*
 *---	women's hats.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1c					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ g++ bespokeHats.cpp -o bespokeHats -lpthread -g -std=c++11

//---	 		    File inclusion:	 			---//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h> // sleep()
#include <vector>

//---	 			Constants:	 			---//

//  PURPOSE:  To tell how many hats to make.
const int NUM_HATS_TO_MAKE = 16;

//  PURPOSE:  To tell the minimum amount of time to make a hat.
const int MIN_NUM_SECS_TO_MAKE_HAT = 5;

//  PURPOSE:  To tell the maximum amount of time to make a hat.
const int MAX_NUM_SECS_TO_MAKE_HAT = 8;

//---	 			Classes:	 			---//

//  PURPOSE:  To hold information on a hat accessory.
class Accessory {
  //  I.  Member vars:
  //  PURPOSE:  To hold the name of the accessory;
  const char *nameCPtr_;

  //  PURPOSE:  To hold the fanciness level (0 is neutral):
  int fancinessLevel_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Accessory();

protected:
  //  III.  Protected methods:

public:
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to have name pointed to by 'newNameCPtr'
  //	and fanciness level 'newFancyLevel'.  No return value.
  Accessory(const char *newNameCPtr, int newFancyLevel)
      : nameCPtr_(newNameCPtr), fancinessLevel_(newFancyLevel) {}

  //  PURPOSE:  To make '*this' a copy of 'source'.  No return value.
  Accessory(const Accessory &source)
      : nameCPtr_(source.getNameCPtr()),
        fancinessLevel_(source.getFancinessLevel()) {}

  //  PURPOSE:  To resources of '*this', make '*this' a copy of 'source',
  //	and return a reference to '*this'.
  Accessory &operator=(const Accessory &source) {
    //  I.  Application validity chech:
    if (this == &source) {
      return (*this);
    }

    //  II.  Release resources:

    //  III.  Copy 'source':
    nameCPtr_ = source.getNameCPtr();
    fancinessLevel_ = source.getFancinessLevel();

    //  IV.  Finished:
    return (*this);
  }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Accessory() {}

  //  V.  Accessors:
  //  PURPOSE:  To return the name of the accessory.  No parameters.
  const char *getNameCPtr() const { return (nameCPtr_); }

  //  PURPOSE:  To return the fanciness level (0 is neutral).  No parameters.
  int getFancinessLevel() const { return (fancinessLevel_); }
};

//  PURPOSE:  To hold the names of the styles of hat.
const char *hatStyleNameArray[] = {
    "Fedora",  "Panama hat", "Boater", "Sun hat",      "Floppy hat",
    "Pillbox", "Beret",      "Bowler", "Baseball cap", "Cowboy hat"};

//  PURPOSE:  To hold the number of elements in 'hatStyleNameArray[]':
const int NUM_HAT_STYLES = sizeof(hatStyleNameArray) / sizeof(const char *);

//  PURPOSE:  To be be the data-structure telling the names and fanciness-
//	levels of the known accessories.
Accessory accessoryArray[] = {
    {"a beer can holder", -10}, {"a propeller", -8},
    {"a miner's lamp", -2},     {"night-vision goggles", +1},
    {"3 small feathers", +6},   {"a veil", +8},
    {"a live pigeon", +10},     {"a live pigeon with its own fedora", +12}};

//  PURPOSE:  To hold the number of elements in 'accessoryArray[]':
const int NUM_ACCESSORIES = sizeof(accessoryArray) / sizeof(Accessory);

//  PURPOSE:  To represent a hat.
class Hat {
  //  I.  Member vars:
  //  PURPOSE:  To keep track of the number of Hat instances that have been
  //	purchased (destroyed).
  static int count__;

  //  PURPOSE:  To hold the index of the hat style.
  int hatStyleIndex_;

  //  PURPOSE:  To hold the accessory(s), if any, on the hat.
  std::vector<const Accessory *> accessoryDs_;

  //  PURPOSE:  To a description of '*this' hat.
  std::string description_;

  //  PURPOSE:  To tell how fancy '*this' hat is.
  int fancinessLevel_;

  //  II.  Disallowed auto-generated methods:
  //  No copy assignment operator:
  Hat &operator=(const Hat &);

protected:
  //  III.  Protected methods:

public:
  //  IV.  Constructor(s),
  //  PURPOSE:  To make an unadorned hat of a random style.  No parameters.
  //	No return value.
  Hat()
      : hatStyleIndex_(rand() % NUM_HAT_STYLES), accessoryDs_(),
        description_(std::string(hatStyleNameArray[hatStyleIndex_])),
        fancinessLevel_(0) {}

  //  PURPOSE:  To make '*this' a copy of 'source'.  No return value.
  Hat(const Hat &source)
      : hatStyleIndex_(source.hatStyleIndex_),
        accessoryDs_(source.accessoryDs_), description_(source.description_),
        fancinessLevel_(source.fancinessLevel_) {}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Hat() { count__++; }

  //  V.  Accessors:
  //  PURPOSE:  To return the number of Hat instances that have been purchased
  //	(destroyed).
  static int getCount() { return (count__); }

  //  PURPOSE:  To return the index of the hat style.  No parameters.
  int getHatStyleIndex() const { return (hatStyleIndex_); }

  //  PURPOSE:  To return a description of '*this' Hat instance as a C-string.
  //	No parameters.
  const char *getDescriptionCPtr() const { return (description_.c_str()); }

  //  PURPOSE:  To return the fanciness level of '*this' Hat instance.
  //	No parameters.
  int getFancinessLevel() const { return (fancinessLevel_); }

  //  VI.  Mutators:

  //  VII.  Methods that do main and misc. work of class:
  //  PURPOSE:  To add 'accessoryPtr' to the data-structure of accessories to
  //	'*this' Hat.
  void decorate(const Accessory *accessoryPtr) {
    if (accessoryDs_.empty()) {
      description_ += " with ";
    } else {
      description_ += " and ";
    }

    description_ += accessoryPtr->getNameCPtr();
    fancinessLevel_ += accessoryPtr->getFancinessLevel();
    accessoryDs_.push_back(accessoryPtr);
  }
};

class Node {
  //  I.  Member vars:
  //  PURPOSE:  To hold the address of the hat.
  Hat *hatPtr_;

  //  PURPOSE:  To hold the address of the next Node in the list, or 'NULL'
  //	if there is no such Node.
  Node *nextPtr_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Node();

  //  No copy constructor:
  Node(const Node &);

  //  No copy assignment op:
  Node &operator=(const Node &);

protected:
  //  III.  Protected methods:

public:
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to hold the address of 'newHatPtr' and
  //	to link to 'newNextPtr'.  No return value.
  Node(Hat *newHatPtr, Node *newNextPtr = NULL)
      : hatPtr_(newHatPtr), nextPtr_(newNextPtr) {}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //  	return value.
  ~Node() { delete (hatPtr_); }

  //  V.  Accessors:
  //  PURPOSE:  To return the address of the hat and make whomever receives it
  //	the new owner of '*hatPtr'.  No parameters.
  Hat *giveAwayHatPtr() {
    Hat *returnMe = hatPtr_;

    hatPtr_ = NULL;
    return (returnMe);
  }

  //  PURPOSE:  To return the address of the next Node in the list, or 'NULL'
  //	if there is no such Node.  No parameters.
  Node *getNextPtr() const { return (nextPtr_); }

  //  VI.  Mutators:
  //  PURPOSE:  To note that 'newNextPtr' is the address of the next Node in
  //	the list.  No return value.
  void setNextPtr(Node *newNextPtr) { nextPtr_ = newNextPtr; }

  //  VII.  Methods that do the main and misc. work of the class:
};

//  PURPOSE:  To store hats.
class HatStore {
  //  I.  Member vars:
  //  PURPOSE:  To tell the maximum length of the list.
  size_t maxLen_;

  //  YOUR LINKED-LIST VARIABLES HERE:
  Node *hatQueue;
  int hatCounter;

  //  YOUR THREAD-SAFETY VARIABLE HERE:
  pthread_mutex_t lock;
  pthread_cond_t finished;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  HatStore();

  //  No copy constructor:
  HatStore(const HatStore &);

  //  No copy assignment op:
  HatStore &operator=(const HatStore &);

protected:
public:
  //  IV.  Constructors:
  //  PURPOSE:  To initialize '*this' to an empty store of addresses of Hat
  //	instances of maximum length 'newMaxLen'.  No return value
  HatStore(size_t newMaxLen) : maxLen_(newMaxLen) {
    // INITIALIZE YOUR LINKED-LIST VARIABLES HERE:
    hatCounter =0;
    hatQueue = NULL;
    

    // INITIALIZE YOUR THREAD-SAFETY VARIABLES HERE:
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&finished,NULL);
  }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //	return value.
  ~HatStore() {
    if (getNumItems() > 0) {
      printf("Too bad: we have to "
             "throw these hats away:\n");

      do {
        delete (retrieve(""));
      } while (getNumItems() > 0);
    }

    // DESTROY YOUR THREAD-SAFETY VARIABLES HERE:
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&finished);
  }

  //  V.  Accessors:
  //  PURPOSE:  To return the number of items in '*this' store.  No parameters.
  int getNumItems() const {
    return (hatCounter); // <-- CHANGE THAT
  }

  //  VI.  Mutators:

  //  VII.  Methods that do misc. and main work of class:
  //  PURPOSE:  To place 'hatPtr' in '*this' store.  Prints out
  //	'waitingMsgCPtr' if forced to wait before doing so.  No return value.
  void store(Hat *hatPtr, const char *waitingMsgCPtr) {
    //  I.  Applicability check:

    //  II.  Place 'hatPtr' in '*this':

    //  YOUR CODE HERE FOR THREAD SAFETY
    pthread_mutex_lock(&lock);
    while (getNumItems() >= maxLen_) {
      printf("%s", waitingMsgCPtr);
      //  YOUR CODE HERE FOR THREAD SAFETY
      pthread_cond_wait(&finished,&lock);
    }

    Node *newNodePtr = new Node(hatPtr);

    //  YOUR CODE HERE TO INSERT 'newNodePtr'
    hatCounter += 1;
    if(hatQueue == NULL) {
      hatQueue = newNodePtr;
    } else {
      Node* current = hatQueue;
      while(current->getNextPtr() != NULL) current= current->getNextPtr();
      current->setNextPtr(newNodePtr);
    }

    //  YOUR CODE HERE FOR THREAD SAFETY
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&finished);

    //  III.  Finished:
  }

  //  PURPOSE:  To get and return the address of a Hat instance from '*this'
  //	store.  Prints out 'waitingMsgCPtr' if forced to wait before doing so.
  Hat *retrieve(const char *waitingMsgCPtr) {
    //  I.  Applicability check:

    //  II.  Place 'hat Ptr' in '*this':
    // this function removes and returns the least recent Hat* in the linked
    // list chang the two ((Node*)NULL to)

    //  YOUR CODE HERE FOR THREAD SAFETY
    pthread_mutex_lock(&lock);
    while (getNumItems() <= 0) {
      printf("%s", waitingMsgCPtr);
      //  YOUR CODE HERE FOR THREAD SAFETY
      pthread_cond_wait(&finished,&lock);
      if (Hat::getCount() >= NUM_HATS_TO_MAKE) {
        return (NULL);
      }
    }

    Hat *returnMe = hatQueue->giveAwayHatPtr(); 
    Node *deleteMe = hatQueue;

    //  YOUR CODE HERE TO REMOVE 'deleteMe' FROM THE LIST:
    hatCounter -= 1;
    hatQueue = hatQueue->getNextPtr();

    delete (deleteMe);
    //  YOUR CODE HERE FOR THREAD SAFETY
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&finished);

    //  III.  Finished:
    return (returnMe);
  }

  void wakeUpAllWaiters() {
    //  YOUR CODE HERE
    //  tell any thread waiting to wake-up with pthread_cond_broadcast()
    pthread_cond_broadcast(&finished);
  }
};

//  PURPOSE:  To hold the addresses of two HatStore instances.
struct HatStoreStore {
  HatStore *store0Ptr_;
  HatStore *store1Ptr_;

  HatStoreStore(HatStore *ptr0, HatStore *ptr1)
      : store0Ptr_(ptr0), store1Ptr_(ptr1) {}
};

//  PURPOSE:  To hold the address of a HatStore instance,
//	and a bool telling whether a shopper want fancy or non-fancy hats.
struct ShopperInfo {
  HatStore *storePtr_;
  bool wantFancy_;

  ShopperInfo(HatStore *ptr, bool wantFancy)
      : storePtr_(ptr), wantFancy_(wantFancy) {}
};

//  PURPOSE:  To keep track of the number of Hat instances that have been
//	purchased (destroyed).
int Hat::count__ = 0;

//  PURPOSE:  To sleep a random amount of time between
//	'MIN_NUM_SECS_TO_MAKE_HAT' and 'MAX_NUM_SECS_TO_MAKE_HAT' seconds.
void sleepRandomAmountOfTime() {
  sleep(MIN_NUM_SECS_TO_MAKE_HAT +
        rand() % (MAX_NUM_SECS_TO_MAKE_HAT - MIN_NUM_SECS_TO_MAKE_HAT + 1));
}

//  PURPOSE:  To make 'NUM_HATS_TO_MAKE' Hat instances and put their addresses
//	in the first hat store.
void *hatMaker(void *vPtr) {
  Hat *hatPtr;
  HatStore *hatStorePtr = (HatStore *)vPtr;

  for (size_t i = 0; i < NUM_HATS_TO_MAKE; i++) {
    printf("Hat maker: \"I'm working on a new hat.\"\n");
    sleepRandomAmountOfTime();
    hatPtr = new Hat(); // CHANGED TO new Hat()
    printf("Hat maker: \"I just made %s\"\n", hatPtr->getDescriptionCPtr());
    //  YOUR CODE HERE
    hatStorePtr->store(hatPtr, "Hat maker: \"Hurry it up with the decorations!  The rack is full!\"\n");
  }

  printf("Hat maker: \"I've made enough hats today, bye!\"\n");
  return (NULL);
}

//  PURPOSE:  To take 'NUM_HATS_TO_MAKE' Hat instances from the first hat
//	store, add either 1 or 2 decorations, and put their addresses in the
//	second hat store.
void *hatDecorator(void *vPtr) {
  Hat *hatPtr;
  HatStoreStore *storeStorePtr = (HatStoreStore *)vPtr;
  HatStore *unfinishedStorePtr = storeStorePtr->store0Ptr_;
  HatStore *finishedStorePtr = storeStorePtr->store1Ptr_;

  for (size_t i = 0; i < NUM_HATS_TO_MAKE; i++) {
    hatPtr = unfinishedStorePtr->retrieve("Decorator: \"Hey hat maker, get to work!\"\n"); //  CHANGE THAT NULL!

    switch (rand() % 2) {
    case 0:
      printf("Decorator: \"I will add one decoration to this %s.\"\n",
             hatPtr->getDescriptionCPtr());
      sleepRandomAmountOfTime();
      // DECORATE THE HAT
      hatPtr->decorate(accessoryArray + (rand() % NUM_ACCESSORIES));
      break;

    case 1:
      printf("Decorator: \"I will add two decorations to this %s.\"\n",
             hatPtr->getDescriptionCPtr());
      sleepRandomAmountOfTime();
      sleepRandomAmountOfTime();
      // DECORATE THE HAT, TWICE
      hatPtr->decorate(accessoryArray + (rand() % NUM_ACCESSORIES));
      hatPtr->decorate(accessoryArray + (rand() % NUM_ACCESSORIES));
      break;
    }

    //  YOUR CODE HERE TO store() THE HAT
    finishedStorePtr->store(hatPtr, "Decorator: \"Hey ladies, start shopping!\"\n");
  }

  printf("Decorator: \"No more decorating, I gotta"
         " clean-up after the remaining pigeons.\"\n");
  return (NULL);
}

//  PURPOSE:  To obtain addresses of Hat instances from the second hat store,
//	and either purchase them if they are liked or throw return them if
//	they are disliked.
void *hatBuyer(void *vPtr) {
  Hat *hatPtr;
  ShopperInfo *infoPtr = (ShopperInfo *)vPtr;
  HatStore *storePtr = infoPtr->storePtr_;
  bool wantFancy = infoPtr->wantFancy_;

  while (Hat::getCount() < NUM_HATS_TO_MAKE) {
    hatPtr = storePtr->retrieve("Shopper: \"I thought there were hats here!?!\"\n"); //  CHANGE THAT NULL

    if (hatPtr == NULL)
      break;

    if ((wantFancy && (hatPtr->getFancinessLevel() >= 0)) ||
        (!wantFancy && (hatPtr->getFancinessLevel() <= 0))) {
      printf("Shopper \"This %s is gorgeous!  I'll take it!\"\n",
             hatPtr->getDescriptionCPtr());
      // GET RID OF THE HAT
      delete hatPtr;
    } else {
      printf("Shopper \"Yuck!  This %s is ugly!  I'll return it!\"\n",f
             hatPtr->getDescriptionCPtr());
      // RETURN hatPtr back to the store
      storePtr->store(hatPtr, "Shopper: \"I thought there were hats here!?!\"\n");
    }

    sleepRandomAmountOfTime();
  }

  printf("Shopper: \"I've done enough damage on my credit card today.\"\n");
  return (NULL);
}

int main(int argc, char *argv[]) {
  HatStore unfinishedHatStore(2);
  HatStore finishedHatStore(NUM_HATS_TO_MAKE / 2);
  HatStoreStore forDecorator(&unfinishedHatStore, &finishedHatStore);
  ShopperInfo shopper0(&finishedHatStore, true);
  ShopperInfo shopper1(&finishedHatStore, false);

  srand(getpid());

  //  YOUR CODE HERE
  pthread_t hat_maker_t;
  pthread_t hat_decorator_t;
  pthread_t shopper1_t;
  pthread_t shopper2_t;
  
  pthread_create(&hat_maker_t, NULL, hatMaker, (void *) &unfinishedHatStore);
  pthread_create(&hat_decorator_t, NULL, hatDecorator, (void *) &forDecorator);
  pthread_create(&shopper1_t, NULL, hatBuyer, (void *) &shopper0);
  pthread_create(&shopper2_t, NULL, hatBuyer, (void *) &shopper1);

  while (Hat::getCount() < NUM_HATS_TO_MAKE) {
    sleep(1);
  }

  finishedHatStore.wakeUpAllWaiters();

  //  YOUR CODE HERE
  pthread_join(hat_maker_t, NULL);
  pthread_join(hat_decorator_t, NULL);
  pthread_join(shopper1_t, NULL);
  pthread_join(shopper2_t, NULL);

  return (EXIT_SUCCESS);
}