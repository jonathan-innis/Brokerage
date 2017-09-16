// Example code from Chapter 16.5 from textbook (chapter.16.7.cpp)
// with additional annotations.
// This code constructs a GUI that interacts with the user through
// buttons, boxes, and menus to draw an open polyline defined by
// points given by the user. 

#include "GUI.h"
#include "Window.h"
#include "std_lib_facilities_4.h"
#include "Brokerage.h"
#include "Graph.h"

struct Brokerage_Window;

using namespace Graph_lib;

double get_double(In_box* i){
	stringstream ss;
	ss << i->get_string();   // input the string containing the floating-point value from the InBox into a stringstream
	float number;
	ss >> number; 
	return number;
}

void brokerage_shutdown(Brokerage broke){
	string output_filename;
	cout << "Enter output File Name as a string with no spaces or special characters (e.g. BrokerageFile1, or Q to quit without saving): " << endl;
	cin >> output_filename;
	if (output_filename == "Q") cout << "Quitting without saving..." << endl;
	else{
		//THIS IS WHERE ALL OF THE FILE GETS OUTPUT
		cout << "Saving Brokerage state to output File " << output_filename << endl;
		cout << "Saving Brokerage name..." << endl;
		cout << "Saving Stock..." << endl;
		cout << "Saving Shares..." << endl;
		cout << "Saving Patrons..." << endl;
		cout << "Saving Transactions..." << endl;
		cout << "Done saving Brokerage state to output File " << output_filename;
		//output_filename += ".txt"; if desired output as text file
		ofstream f;
		f.open(output_filename);
		f << broke;
		for(int i = 0; i < 5; i++){
			f << "s " << broke.shares_info(i).type() << endl;
		}
		for (int i = 0; i < 5; i++){
			f << "S " << broke.shares_info(i) << endl;
		}
		f << "C " << broke.cash() << endl;
		f << "U " << broke.USD() << endl;
		for (int i = 0; i < broke.patron_size(); i++){
			f << "P " << broke.patron(i) << endl;
		}
		for (int i = 0; i < broke.transactions_size(); i++){
			if (i == broke.transactions_size() - 1) f << "X " << broke.transaction(i);
			else f << "X " << broke.transaction(i) << endl;
		}
	}
}

//----------------------------------------------------------
// define a struct that is a window in which lines can be
// entered via a GUI

vector<string> broke_names = {};
vector<Brokerage*> brokes = {};
vector<Brokerage_Window*> windows = {};
int transfer_number;
string transfer_name;
double transfer_amount;
string transfer_receiving;

struct Brokerage_Window : Graph_lib::Window {       // inherits from Window

  // constructor
  Brokerage_Window(Point xy,             // top lefthand corner
	       int w,                // width
	       int h,                // height
	       const string& title, Brokerage& b); // label
	
	Brokerage& broke;
	char action;
private:
  // widgets:
  Button next_button;                // button indicating next point is ready
  Button quit_button;                // end program
  In_box I2;                     
  In_box I3;                      
  Out_box I1;                    
  Menu brokerage_menu;               // menu of color choices for the line
  Button menu_button;                // button to display the color menu
  In_box I4;
  In_box I5;
  In_box I6;
  Out_box INTC;
  Out_box GOOG;
  Out_box AAPL;
  Out_box YHOO;
  Out_box IBM;
  Out_box USDs;
  Out_box O1;
  Out_box O2;
  Out_box O3;
  Out_box O4;
  Out_box O5;
  Out_box O6;
  Out_box L1;
  Out_box L2;
  Out_box L3;
  Out_box L4;
  Out_box L5;
  Out_box L6;
  Out_box L7;
  Out_box L8;
  Out_box L9;
  Out_box L10;

  void hide_menu() {     
    // hides the color menu and shows the button to display the color menu
    brokerage_menu.hide(); 
    menu_button.show(); 
  }

  // actions invoked by callbacks:

	void show_brokerage();
  void info_pressed();
  void add_patron_pressed();
	void check_patron_pressed();
	void display_patrons_pressed();
	void add_cash_pressed();
	void remove_cash_pressed();
	void shares_sale_pressed();
	void shares_buy_pressed();
	void overdrawn_patrons_pressed();
	void display_transactions_pressed();
	void transfer_cash_pressed();

  void menu_pressed() {
    // when menu button is pressed, hide the menu button and show the 
    // actual menu of colors
    menu_button.hide();    
    brokerage_menu.show();
  }

  void next();   // defined below

  void quit();   // defined below

  // callback functions; declared here and defined below.

  static void cb_info(Address, Address);
  static void cb_add_patron(Address, Address);
  static void cb_check_patron(Address, Address);
  static void cb_display_patrons(Address, Address);
  static void cb_add_cash(Address, Address);
  static void cb_remove_cash(Address, Address);
  static void cb_shares_sale(Address, Address);
  static void cb_shares_buy(Address, Address);
  static void cb_overdrawn_patrons(Address, Address);
  static void cb_display_transactions(Address, Address);
  static void cb_transfer_cash(Address, Address);
  
  static void cb_menu(Address, Address);
  static void cb_next(Address, Address);
  static void cb_quit(Address, Address);
};

// ----------------------------------------------------------
// now define the parts of Brokerage_Window that were only declared
// inside the class

// constructor:

Brokerage_Window::Brokerage_Window(Point xy, int w, int h, const string& title, Brokerage& b) : 

  // initialization - start by calling constructor of base class 
  Window(xy,w,h,title), broke(b),

  // initialize "Next point" button
  next_button(
	      Point(x_max()-150,0),   // location of button
	      70, 20,                 // dimensions of button
	      "Next",           // label of button
	      cb_next),               // callback function for button
  // initialize quit button
  quit_button(
	      Point(x_max()-70,0),    // location of button
	      70, 20,                 // dimensions of button 
	      "Quit",                 // label of button
	      cb_quit),               // callback function for button
 
 	I2(Point(x_max() - 4*(x_max()/6)-70-120, 30), (x_max()/6)-5, 20, "I2: "),
	I3(Point(x_max() - 3*(x_max()/6)-70-80, 30), (x_max()/6)-5, 20, "I3: "),
	I4(Point(x_max() - 2*(x_max()/6)-70-40, 30), (x_max()/6)-5, 20, "I4: "),
	I5(Point(x_max() - (x_max()/6)-70, 30), (x_max()/6)-5, 20, "I5: "),
	
	I6(Point(40,60), (x_max() - 115), 20, "I6: "),
	

//OUT_BOXES
	I1(Point(40,30), (x_max()/6)-5, 20, "I1: "),
	
	O1(Point(40,90), (x_max()/6)-5, 20, "O1: "),
	
	O2(Point(x_max() - 4*(x_max()/6)-70-120, 90), (x_max()/6)-5, 20, "O2: "),
	O3(Point(x_max() - 3*(x_max()/6)-70-80, 90), (x_max()/6)-5, 20, "O3: "),
	O4(Point(x_max() - 2*(x_max()/6)-70-40, 90), (x_max()/6)-5, 20, "O4: "),
	O5(Point(x_max() - (x_max()/6)-70, 90), (x_max()/6)-5, 20, "O5: "),
	
	O6(Point(40,120), (x_max() - 115), 20, "O6: "),
	
	L1(Point(40,150), (x_max() - 115), 20, "L1: "),
	L2(Point(40,180), (x_max() - 115), 20, "L2: "),
	L3(Point(40,210), (x_max() - 115), 20, "L3: "),
	L4(Point(40,240), (x_max() - 115), 20, "L4: "),
	L5(Point(40,270), (x_max() - 115), 20, "L5: "),
	L6(Point(40,300), (x_max() - 115), 20, "L6: "),
	L7(Point(40,330), (x_max() - 115), 20, "L7: "),
	L8(Point(40,360), (x_max() - 115), 20, "L8: "),
	L9(Point(40,390), (x_max() - 115), 20, "L9: "),
	L10(Point(40,420), (x_max() - 115), 20, "L10: "),
	
	INTC(Point(40,0), (x_max()/9), 20, "INTC"),
	GOOG(Point(40+x_max()/9+50,0), (x_max()/8), 20, "GOOG"),
	AAPL(Point(40+2*x_max()/9+110,0), (x_max()/8), 20, "AAPL"),
	YHOO(Point(40+3*x_max()/9+170,0), (x_max()/8), 20, "YHOO"),
	IBM(Point(40+4*x_max()/9+230,0), (x_max()/8), 20, "IBM"),
	USDs(Point(40+5*x_max()/9+290,0), (x_max()/8), 20, "USDs"),

  
  brokerage_menu(                        
	     Point(x_max()-70,30),   // location of menu
	     70, 20,                 // dimensions of menu
	     Menu::vertical,         // list menu items vertically
	     "Menu"),               // label of menu 
  // initialize the menu button
  menu_button(
	      Point(x_max()-70,30),  // location of menu button
	      80, 20,                // dimensions of button 
	      "Menu",          // label of button
	      cb_menu)              // callback for button
  // body of constructor follows
{
  // attach buttons and boxes to window
  attach(next_button);
  attach(quit_button);
  attach(I1);
  attach(I2);
  attach(I3);
  attach(I4);
  attach(I5);
  attach(I6);
  attach(O1);
  attach(O2);
  attach(O3);
  attach(O4);
  attach(O5);
  attach(O6);
  attach(L1);
  attach(L2);
  attach(L3);
  attach(L4);
  attach(L5);
  attach(L6);
  attach(L7);
  attach(L8);
  attach(L9);
  attach(L10);
  attach(INTC);
  attach(GOOG);
  attach(AAPL);
  attach(YHOO);
  attach(IBM);
  attach(USDs);

  // First make 3 buttons for color menu, one for each color, and 
  // attach them to the menu: the attach function of the Menu struct
  // adjusts the size and location of the buttons; note callback functions).
  // Then attach menu to window but hide it (initially, the menu button
  // is displayed, not the actual menu of color choices).

  brokerage_menu.attach(new Button(Point(0,0),0,0,"ShowBrokerage",cb_info)); 
  brokerage_menu.attach(new Button(Point(0,0),0,0,"AddPatron",cb_add_patron));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"Is?Patron",cb_check_patron));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"Patrons",cb_display_patrons));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"AddCash",cb_add_cash));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"RemCash",cb_remove_cash));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"SellStock",cb_shares_sale));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"BuyStock",cb_shares_buy));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"DeadBeats",cb_overdrawn_patrons));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"Xactions",cb_display_transactions));
  brokerage_menu.attach(new Button(Point(0,0),0,0,"Transfer",cb_transfer_cash));
  
  attach(brokerage_menu);
  brokerage_menu.hide(); 

  // attach menu button
  attach(menu_button);
}

// ---------------------------- 
// callback function for quit button - boilerplate: 
// When the button is pressed, the system invokes the
// specified callback function.  First argument is address of the
// button (which we won't use, so we don't bother to name it).  Second
// argument, named pw, is address of the window containing the pressed
// button, i.e., address of our Brokerage_Window object.  reference_to
// converts the address pw into a reference to a Brokerage_Window object,
// so we can call the quit() function.  Objective is to call function
// quit() which does the real work specific to this button.

void Brokerage_Window::show_brokerage(){
	I1.put("ShowBrokerage");
	stringstream ss;
	ss << broke.shares_info(0);
	INTC.put(ss.str().erase(0,5));
	ss.str("");
	
	ss << broke.shares_info(1);
	GOOG.put(ss.str().erase(0,5));
	ss.str("");
	
	ss << broke.shares_info(2);
	AAPL.put(ss.str().erase(0,5));
	ss.str("");
	
	ss << broke.shares_info(3);
	YHOO.put(ss.str().erase(0,5));
	ss.str("");
	
	ss << broke.shares_info(4);
	IBM.put(ss.str().erase(0,4));
	ss.str("");
	
	ss << broke.cash() << " " << broke.USD();
	USDs.put(ss.str());
	ss.str("");
}

void Brokerage_Window::cb_quit(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).quit();   // quit is defined next
}

//------------------------------------
// what to do when quit button is pressed

void Brokerage_Window::quit() {
  hide();                   // FLTK idiom for delete window
  brokerage_shutdown(broke);
}

// ----------------------------
// callback function for next button - boilerplate:

void Brokerage_Window::cb_next(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).next();  // next is defined next
}

// -------------------------------
// what to do when "next point" button is pressed

void Brokerage_Window::next() {
	stringstream ss;
	ss.str("");
  INTC.put(ss.str());
  GOOG.put(ss.str());
  AAPL.put(ss.str());
  YHOO.put(ss.str());
  IBM.put(ss.str());
  USDs.put(ss.str());
  O1.put(ss.str());
  O2.put(ss.str());
  O3.put(ss.str());
  O4.put(ss.str());
  O5.put(ss.str());
  O6.put(ss.str());
  L1.put(ss.str());
  L2.put(ss.str());
  L3.put(ss.str());
  L4.put(ss.str());
  L5.put(ss.str());
  L6.put(ss.str());
  L7.put(ss.str());
  L8.put(ss.str());
 	L9.put(ss.str());
	
	switch (action){
		//Show Brokerage------------------------------------------------------
		case 'A':
		{
			show_brokerage();
			break;
		}
		//Add Patron------------------------------------------------------
		case 'B':
		{
			string name = I2.get_string();
			int number = I3.get_int();
			if (number < 0) {
				O2.put("AddPatron FAILED!");
				O1.put("AddPatron");
				show_brokerage();
				break;
			}
			In_box* I = &I4;
			double amount = get_double(I);
			if (amount < 0) {
				O1.put("AddPatron");
				O2.put("AddPatron FAILED!");
				show_brokerage();
				break;
			}
			Patron p(name,number,amount);
			broke.addPatron(p);
			broke.add_cash(amount);
			stringstream ss;
			
			ss << name;
			O1.put("AddPatron");
			O2.put(ss.str());
			ss.str("");
			
			ss << number;
			O3.put(ss.str());
			ss.str("");
			
			ss << amount;
			O4.put(ss.str());
			ss.str("");
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		//Is Patron------------------------------------------------------
		case 'C':
		{
			stringstream ss;
			int number = I2.get_int();
			if (number < 0) {
				O1.put("IsPatron?");
				O2.put("CheckPatron FAILED!");
				show_brokerage();
				break;
			}
			int index = broke.find_patron(number);
			if (index == -1) O2.put("IsPatron? Failed!");
			else{
				ss << number;
				O2.put(ss.str());
				ss.str("");
				
				ss << broke.patron(index);
				O6.put(ss.str());
				ss.str("");
			}
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			
			O1.put("IsPatron?");
			break;
		}
		//Display Patrons------------------------------------------------------
		case 'D':
		{
			stringstream ss;
			Out_box* b1 = &L1;
			Out_box* b2 = &L2;
			Out_box* b3 = &L3;
			Out_box* b4 = &L4;
			Out_box* b5 = &L5;
			Out_box* b6 = &L6;
			Out_box* b7 = &L7;
			Out_box* b8 = &L8;
			Out_box* b9 = &L9;
			Out_box* b10 = &L10;
			vector<Out_box*> boxes = {b1,b2,b3,b4,b5,b6,b7,b8,b9,b10};
			
			for (int i = 0; i < broke.patron_size(); i++){
				if (i > 9) break;
				ss << broke.patron(i);
				boxes.at(i)->put(ss.str());
				ss.str("");
			}
			O1.put("Patrons");
			ss << broke.patron_size();
			O2.put(ss.str());
			ss.str("");
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		//Add Cash------------------------------------------------------
		case 'E':
		{
			stringstream ss;
			int number = I2.get_int();
			if (number < 0) {
				O1.put("AddCash");
				O2.put("AddCash FAILED!");
				show_brokerage();
				break;
			}
			In_box* I = &I3;
			double amount = get_double(I);
			if (amount < 0) {
				O1.put("AddCash");
				O2.put("AddCash FAILED!");
				show_brokerage();
				break;
			}				
			
			int index = broke.find_patron(number);
			broke.patron(index).add_cash(amount);
			broke.add_cash(amount);
			
			O1.put("AddCash");
			ss << number;
			O2.put(ss.str());
			ss.str("");
			
			ss << amount;
			O3.put(ss.str());
			ss.str("");
			
			ss << broke.patron(index);
			O6.put(ss.str());
			ss.str("");
			
			//Adding the Transaction
			Chrono::Time t;
			Chrono::Date d;
			broke.addTransaction(Transaction(broke.patron(index),'c',"Cash_Deposit",amount,t,d));
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			ss.str("");
			
			break;
		}
		//Remove Cash------------------------------------------------------
		case 'F':
		{
			stringstream ss;
			int number = I2.get_int();
			if (number < 0) {
				O1.put("RemCash");
				O2.put("RemCash FAILED!");
				show_brokerage();
				break;
			}
			In_box* I = &I3;
			double amount = get_double(I);
			if (amount < 0) {
				O1.put("RemCash");
				O2.put("RemCash FAILED!");
				show_brokerage();
				break;
			}
			
			int index = broke.find_patron(number);
			broke.patron(index).remove_cash(amount);
			broke.remove_cash(amount);
			
			O1.put("RemCash");
			ss << number;
			O2.put(ss.str());
			ss.str("");
			
			ss << amount;
			O3.put(ss.str());
			ss.str("");
			
			ss << broke.patron(index);
			O6.put(ss.str());
			ss.str("");
			
			//Adding the Transaction
			Chrono::Time t;
			Chrono::Date d;
			broke.addTransaction(Transaction(broke.patron(index),'c',"Cash_Withdrawl",amount,t,d));
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			
			break;
		}
		//Sell Stock------------------------------------------------------
		case 'G':
		{
			bool failed = false;
			stringstream ss;
			int stock_number = -1;
			int number = I2.get_int();
			string stock_string = I3.get_string();
			O1.put("SellStock");
			if (stock_string == ""){
				O2.put("SellStock FAILED!");
				show_brokerage();
				break;
			}
			
			char stock_type = stock_string[0];
			In_box* I = &I4;
			double amount = get_double(I);
			if (amount < 0){
				O2.put("SellStock FAILED!");
				show_brokerage();
				break;
			}
			
			int index = broke.Brokerage::find_patron(number);
			
			//declaring the type of stock
			switch (stock_type) {
				case 'i':
					stock_number = 0;
					break;
				case 'g': 
					stock_number = 1;
					break;
				case 'a':
					stock_number = 2;
					break;
				case 'y':
					stock_number = 3;
					break;
				case 'I':
					stock_number = 4;
					break;
				default:
					O2.put("SellStock FAILED");
					failed = true;
					break;
			}
			if (failed == true){
				show_brokerage();
				break;
			}
			
			double patron_total_shares = broke.patron(index).num_shares(stock_number);
			double broke_total_shares = broke.shares_info(stock_number).number();
			
			broke.patron(index).update_shares(stock_number, patron_total_shares - amount);
			broke.shares_info(stock_number).update(broke_total_shares - amount);
			
			broke.patron(index).add_cash((amount * broke.shares_info(stock_number).type().price()));
			broke.add_cash(amount * broke.shares_info(stock_number).type().price());
			
			ss << number;
			O2.put(ss.str());
			ss.str("");
			
			ss << amount;
			O3.put(ss.str());
			ss.str("");
			
			ss << broke.patron(index);
			O6.put(ss.str());
			ss.str("");
			
			Chrono::Time t;
			Chrono::Date d;
			broke.addTransaction(Transaction(broke.patron(index),stock_number,"Stock_Sale",amount,t,d));
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		//Buy Stock------------------------------------------------------
		case 'H':
		{
			bool failed = false;
			stringstream ss;
			int stock_number = -1;
			int number = I2.get_int();
			string stock_string = I3.get_string();
			O1.put("BuyStock");
			if (stock_string == ""){
				O2.put("BuyStock FAILED!");
				show_brokerage();
				break;
			}
			char stock_type = stock_string[0];
			In_box* I = &I4;
			double amount = get_double(I);
			if (amount < 0){
				O2.put("BuyStock FAILED!");
				show_brokerage();
				break;
			}
			
			int index = broke.Brokerage::find_patron(number);
			
			//declaring the type of stock
			switch (stock_type) {
				case 'i':
					stock_number = 0;
					break;
				case 'g': 
					stock_number = 1;
					break;
				case 'a':
					stock_number = 2;
					break;
				case 'y':
					stock_number = 3;
					break;
				case 'I':
					stock_number = 4;
					break;
				default:
					O2.put("BuyStock FAILED");
					failed = true;
					break;
			}
			
			if (failed == true){
				show_brokerage();
				break;
			}
			
			double patron_total_shares = broke.patron(index).num_shares(stock_number);
			double broke_total_shares = broke.shares_info(stock_number).number();
			
			broke.patron(index).update_shares(stock_number, patron_total_shares + amount);
			broke.shares_info(stock_number).update(broke_total_shares + amount);
			
			broke.patron(index).remove_cash((amount * broke.shares_info(stock_number).type().price()));
			broke.remove_cash(amount * broke.shares_info(stock_number).type().price());
			
			ss << number;
			O2.put(ss.str());
			ss.str("");
			
			ss << amount;
			O3.put(ss.str());
			ss.str("");
			
			ss << broke.patron(index);
			O6.put(ss.str());
			ss.str("");
			
			Chrono::Time t;
			Chrono::Date d;
			broke.addTransaction(Transaction(broke.patron(index),stock_number,"Stock_Buy",amount,t,d));
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		//Display Deadbeats------------------------------------------------------
		case 'I':
		{
			stringstream ss;
			Out_box* b1 = &L1;
			Out_box* b2 = &L2;
			Out_box* b3 = &L3;
			Out_box* b4 = &L4;
			Out_box* b5 = &L5;
			Out_box* b6 = &L6;
			Out_box* b7 = &L7;
			Out_box* b8 = &L8;
			Out_box* b9 = &L9;
			Out_box* b10 = &L10;
			vector<Out_box*> boxes = {b1,b2,b3,b4,b5,b6,b7,b8,b9,b10};
			
			
			int num_dead = 0;
			for (int i = 0; i < broke.patron_size(); i++){
				if (broke.patron(i).cash() < 0){
					ss << broke.patron(i);
					boxes.at(i)->put(ss.str());
					num_dead++;
					ss.str("");
				}
			}
			O1.put("DeadBeats");
			ss << num_dead;
			O2.put(ss.str());
			ss.str("");
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		case 'J':
		{
			stringstream ss;
			Out_box* b1 = &L1;
			Out_box* b2 = &L2;
			Out_box* b3 = &L3;
			Out_box* b4 = &L4;
			Out_box* b5 = &L5;
			Out_box* b6 = &L6;
			Out_box* b7 = &L7;
			Out_box* b8 = &L8;
			Out_box* b9 = &L9;
			Out_box* b10 = &L10;
			vector<Out_box*> boxes = {b1,b2,b3,b4,b5,b6,b7,b8,b9,b10};
			for (int i = 0; i < broke.transactions_size(); i++){
				ss << broke.transaction(i);
				boxes.at(i)->put(ss.str());
				ss.str("");
			}
			ss << broke.transactions_size();
			O2.put(ss.str());
			ss.str("");
			
			//Displaying the Brokerage State-----------------------
			show_brokerage();
			break;
		}
		//Transfer Funds------------------------------------------------------
		case 'K':
		{
			int i;
			int window_number;
			stringstream ss;
			bool succesful = false;
			int number = I2.get_int();
			In_box* I = &I3;
			double amount = get_double(I);
			string brokerage_name = I4.get_string();
			transfer_number = number;
			transfer_name = broke.name();
			transfer_receiving = brokerage_name;
			transfer_amount = amount;
			
			
			O1.put("Transfer num,amt,dst");
			for (i = 0; i < 3; i++){
				if ((brokes.at(i))->name() == brokerage_name){
					window_number = i;
					if ((brokes.at(i))->find_patron(number) != -1){
						int index = broke.find_patron(number);
						if (broke.patron(index).cash() >= amount){ //won't let patron remove more money than person has
							succesful = true;
							ss << number;
							O2.put(ss.str());
							ss.str("");
							
							ss << broke.name();
							O3.put(ss.str());
							ss.str("");
							
							ss << amount;
							O4.put(ss.str());
							ss.str("");
							
							ss << brokes.at(i)->name();
							O5.put(ss.str());
							ss.str("");
							
							int index = broke.find_patron(number);
							broke.patron(index).remove_cash(amount);
							broke.remove_cash(amount);
							int index2 = (brokes.at(i))->find_patron(number);
							(brokes.at(i))->patron(index2).add_cash(amount);
							(brokes.at(i))->add_cash(amount);
							
							ss << broke.patron(index);
							O6.put(ss.str());
							ss.str("");
							
							windows.at(i)->action = 'Y';
						}
					}
				}
			}
			if (!succesful){
				O2.put("TansferIn FAILED!!");
				action = 'Z';
				windows.at(window_number)->action = 'X';
			}
			show_brokerage();
			break;		
		}
		case 'X':{
			O1.put("TransferIn");
			O2.put("TransferIn FAILED!!");
			break;
		}
		case 'Y':{
			stringstream ss;
			
			O1.put("TransferIn");
			
			ss << transfer_number;
			O2.put(ss.str());
			ss.str("");
			
			ss << transfer_name;
			O3.put(ss.str());
			ss.str("");
			
			ss << transfer_amount;
			O4.put(ss.str());
			ss.str("");
			
			ss << transfer_receiving;
			O5.put(ss.str());
			ss.str("");
			
			ss << broke.patron(broke.find_patron(transfer_number));
			O6.put(ss.str());
			ss.str("");
			show_brokerage();
			break;
		}
		case 'Z':{
			O1.put("TransferIn");
			
			ss << transfer_number;
			O2.put(ss.str());
			ss.str("");
			
			ss << transfer_name;
			O3.put(transfer_name);
			ss.str("");
			
			ss << transfer_amount;
			O4.put(ss.str());
			ss.str("");
			
			ss << transfer_name;
			O5.put(ss.str());
			ss.str("");
			
			ss << broke.patron(broke.find_patron(transfer_number));
			O6.put(ss.str());
			ss.str("");
			show_brokerage();
			break;
		}
	}
}

void Brokerage_Window::info_pressed()
	{
		action = 'A';
		I1.put("ShowBrokerage");
		hide_menu();
	}
void Brokerage_Window::add_patron_pressed()
	{
		action = 'B'; //adding a patron
		I1.put("AddPatron nam,num,amt");
		hide_menu();
	}
void Brokerage_Window::check_patron_pressed()
	{
		action = 'C'; //checking a patron
		I1.put("IsPatron? num");
		hide_menu();
	}
void Brokerage_Window::display_patrons_pressed()
	{
		action = 'D'; //displaying patrons
		I1.put("Patrons");
		hide_menu();
	}
void Brokerage_Window::add_cash_pressed()
	{
		action = 'E'; //adding cash
		I1.put("AddCash num amt");
		hide_menu();
	}
void Brokerage_Window::remove_cash_pressed()
	{
		action = 'F'; //removing cash
		I1.put("RemCash num amt");
		hide_menu();
	}
void Brokerage_Window::shares_sale_pressed()
	{
		action = 'G'; //selling shares
		I1.put("SellStock num,stk,amt");
		hide_menu();
	}
void Brokerage_Window::shares_buy_pressed()
	{
		action = 'H'; //buying shares
		I1.put("BuyStock num,stk,amt");
		hide_menu();
	}
void Brokerage_Window::overdrawn_patrons_pressed()
	{
		action = 'I'; //showing overdrawn patrons
		I1.put("DeadBeats");
		hide_menu();
	}
void Brokerage_Window::display_transactions_pressed()
	{
		action = 'J'; //displaying transactions
		I1.put("Xactions");
		hide_menu();
	}
void Brokerage_Window::transfer_cash_pressed()
	{
		action = 'K';
		I1.put("Transfer num,amt,dst");
		hide_menu();
	}

//--------------------------------------------------------

void Brokerage_Window::cb_info(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).info_pressed();  
}

void Brokerage_Window::cb_add_patron(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).add_patron_pressed();  
}

void Brokerage_Window::cb_check_patron(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).check_patron_pressed();  
}

void Brokerage_Window::cb_display_patrons(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).display_patrons_pressed();  
}

void Brokerage_Window::cb_add_cash(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).add_cash_pressed();  
}

void Brokerage_Window::cb_remove_cash(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).remove_cash_pressed();  
}

void Brokerage_Window::cb_shares_sale(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).shares_sale_pressed();  
}

void Brokerage_Window::cb_shares_buy(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).shares_buy_pressed();  
}

void Brokerage_Window::cb_overdrawn_patrons(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).overdrawn_patrons_pressed();  
}

void Brokerage_Window::cb_display_transactions(Address, Address pw) {
  reference_to<Brokerage_Window>(pw).display_transactions_pressed();  
}

void Brokerage_Window::cb_transfer_cash(Address, Address pw) {
	reference_to<Brokerage_Window>(pw).transfer_cash_pressed();
}

// -------------------------------
// callback for when menu button is pressed - boilerplate

void Brokerage_Window::cb_menu(Address, Address pw)
{  
    reference_to<Brokerage_Window>(pw).menu_pressed();
    // menu_pressed defined in Brokerage_Window class as part of declaration
} 

// ---------------------------------------------------
// main - just creates window and invokes gui_main

Brokerage brokerage_setup(){
	string brokerage_name;
	string filename = "";
	bool unique_name = true;
	
	cout << "Setting Name of Brokerage..." << endl;
	cout << "Enter Brokerage name as a string (no spaces): ";
	cin >> brokerage_name;
	for (int i = 0; i < broke_names.size(); i++){
		if (broke_names.at(i) == brokerage_name) unique_name = false;
	}
	if (unique_name) broke_names.push_back(brokerage_name);
	else error("This name has already been taken.");	
	cout << "Enter input File Name as a string with no spaces or special characters (e.g. BrokerFile1, or Q to skip File read): ";
	
	cin >> filename;
	
	if (filename == "Q"){
		cout << "Skipping input File read and instead initializing a new Brokerage state" << endl;
		Brokerage broke(brokerage_name);
		return broke;
	}
	else{	
		ifstream f;
		string object;
		string name;
		Shares new_share;
		Patron new_patron;
		Transaction new_trans;
		double cash;
		double USD;
		vector<Shares> vec_shares;
		vector<Patron> vec_patron;
		vector<Transaction> vec_trans;
		//filename += ".txt"; if desired to input as a text file
		f.open(filename);
		
		if (!f.good()) {
			cerr << "Error: Can't open input file " << filename;
			throw Filename_error();
		}
		
		while (!f.eof() || f.good()){
			f >> object;
			if (object == "B") f >> name;
			if (object == "S"){
				f >> new_share;
				vec_shares.push_back(new_share);
			}
			if (object == "P"){
				f >> new_patron;
				vec_patron.push_back(new_patron);
			}
			if (object == "X"){
				f >> new_trans;
				vec_trans.push_back(new_trans);
			}
			else if (object == "C") f >> cash;
			else if (object == "U") f >> USD;
		}
		cout << "Loading initial Brokerage state from input File " << filename << endl;
		cout << "Done initializing Brokerage state from input File " << filename << endl;
		Brokerage broke(name, vec_patron, vec_trans, cash, vec_shares);
		return broke;
	}
}

int main(){ 
  try {
  	Brokerage b1 = brokerage_setup();
  	Brokerage* B1 = &b1;
  	brokes.push_back(B1);

  	Brokerage b2 = brokerage_setup();
  	Brokerage* B2 = &b2;
  	brokes.push_back(B2);
  	
  	Brokerage b3 = brokerage_setup();
  	Brokerage* B3 = &b3;
  	brokes.push_back(B3);
  	
    // construct the GUI window
    Brokerage_Window win1(Point(100,100),x_max(),y_max()/1.75, "Brokerage 1 - " + b1.name(),b1);
	Brokerage_Window win2(Point(100,150),x_max()-1,y_max()/1.75, "Brokerage 2 - " + b2.name(),b2);
	Brokerage_Window win3(Point(100,200),x_max()-2,y_max()/1.75, "Brokerage 3 - " + b3.name(),b3);
	
	Brokerage_Window* W1 = &win1;
	windows.push_back(W1);
	Brokerage_Window* W2 = &win2;
	windows.push_back(W2);
	Brokerage_Window* W3 = &win3;
	windows.push_back(W3);
	
    return gui_main();  // inherited from Window; calls FLTK's run
  }
  catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
  }
  /*catch(...) {
    cerr << "some exception\n";
    return 2;
  }*/
  return 0;
}