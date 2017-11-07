#include "includes/General/Utility/HandlerBlock.h"
#include "includes/General/Utility/TaskBlock.h"
#include "includes/General/Utility/CallableChain.h"
#include "includes/General/Utility/TaskPriorityBlock.h"
#include "includes/General/Utility/HandlerPriorityBlock.h"
#include "includes/General/Utility/CommandBlock.h"
#include "includes/General/Utility/BranchingBlock.h"
#include "includes/General/Utility/TaskQueueBlock.h"


#include <iostream>
#include <string>
#include <vector>



using namespace IS::General::Utility;


int main () {
  auto upperNumber = [](double &d, double h){
    d = d+h;
  };
  auto productNumber = [](double &d, double h){
    d = d*h;
  };

  double a = 2.0, b = 2.0;

  auto handlerBlock_1 = HandlerBlock<double>();
  handlerBlock_1.addHandler(upperNumber, 2);
  handlerBlock_1.addHandler(upperNumber, 3);
  handlerBlock_1.addHandler(upperNumber, 4);
  handlerBlock_1.addHandler(upperNumber, 5);

  auto handlerBlock_2 = HandlerBlock<double>();
  handlerBlock_2.addHandler(productNumber, 2);
  handlerBlock_2.addHandler(productNumber, 3);
  handlerBlock_2.addHandler(productNumber, 4);
  handlerBlock_2.addHandler(productNumber, 5);

  auto handlerBlock_12 = HandlerBlock<double>();
  handlerBlock_12.addHandler(handlerBlock_1);
  handlerBlock_12.addHandler(handlerBlock_2, true);
  auto caller_12_1 = handlerBlock_12.getBindingFunction(a);
  handlerBlock_12.addHandler(productNumber, 4);
  handlerBlock_12.addHandler(upperNumber, 10);
  auto caller_12_2 = handlerBlock_12.getBindingFunction(b);


  auto taskBlock = TaskBlock();
  taskBlock.addTask(handlerBlock_1.getBindingFunction(a));
  taskBlock.addTask(handlerBlock_2.getBindingFunction(a));

  auto taskPriorityBlock = TaskPriorityBlock<>();
  taskPriorityBlock.addTask(2, handlerBlock_1.getBindingFunction(b));
  taskPriorityBlock.addTask(1, handlerBlock_2.getBindingFunction(b));

  taskBlock();
  taskPriorityBlock();
  std::cout<<a<<std::endl;
  std::cout<<b<<std::endl;


  auto handlerPriorityBlock_1 = HandlerPriorityBlock<double>();
  auto handlerPriorityBlock_2 = HandlerPriorityBlock<double>();
  handlerPriorityBlock_1.addHandler(2, productNumber, 10);
  handlerPriorityBlock_1.addHandler(1, upperNumber, 10);
  handlerPriorityBlock_2.addHandler(1, productNumber, 10);
  handlerPriorityBlock_2.addHandler(2, upperNumber, 10);

  double c = 1.0, d = 1.0;

  handlerPriorityBlock_1(c);
  handlerPriorityBlock_2(d);
  std::cout<<c<<", "<<d<<std::endl;


  auto taskQueue = TaskQueueBlock();
  taskQueue.addTask([](){std::cout<<"A"<<std::endl;});
  taskQueue.addTask([](){std::cout<<"B"<<std::endl;});
  taskQueue.addTask([](){std::cout<<"C"<<std::endl;});
  taskQueue.addTask([](){std::cout<<"D"<<std::endl;});
  taskQueue();
  taskQueue.addTask([](){std::cout<<"E"<<std::endl;});
  taskQueue.addTask([](){std::cout<<"F"<<std::endl;});
  taskQueue();


  auto taskChooseRadio = [](){std::cout<<"RADIO IS CHOOSEN"<<std::endl;};
  auto taskChooseFM = [](){std::cout<<"RADIO TURN ON FM"<<std::endl;};
  auto taskNextStation = [](){std::cout<<"NEXT STATION IS CHOOSEN"<<std::endl;};
  auto taskPrevStation = [](){std::cout<<"PREV STATION IS CHOOSEN"<<std::endl;};
  auto taskVolumeUp = [](){std::cout<<"VOLUME IS UPPER"<<std::endl;};
  auto taskVolumeDown = [](){std::cout<<"VOLUME IS LOWER"<<std::endl;};
  auto taskRadioPlay = [](){std::cout<<"RADIO IS PLAYED"<<std::endl;};
  auto taskRadioStop = [](){std::cout<<"RADIO IS STOPPED"<<std::endl;};

  auto branchPlayer = BranchingBlock<std::string>();
  auto branchRadio = BranchingBlock<std::string>();
  branchPlayer.addBranch("RADIO", taskChooseRadio, branchRadio);
  branchRadio.addBranch("FM", taskChooseFM, branchRadio);
  branchRadio.addBranch("NEXT", taskNextStation, branchRadio);
  branchRadio.addBranch("PREV", taskPrevStation, branchRadio);
  branchRadio.addBranch("UP", taskVolumeUp, branchRadio);
  branchRadio.addBranch("DOWN", taskVolumeDown, branchRadio);
  branchRadio.addBranch("PLAY", taskRadioPlay, branchRadio);
  branchRadio.addBranch("STOP", taskRadioStop, branchRadio);

  branchRadio = branchRadio.back()("RADIO")("FM");
  branchRadio = branchRadio("NEXT")("NEXT")("PREV");
  branchRadio = branchRadio("UP")("UP")("UP")("PLAY")("DOWN")("STOP").back();
  branchRadio("FM");
}
