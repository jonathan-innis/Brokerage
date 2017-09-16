

#include "std_lib_facilities_4.h"
#include "Brokerage.h"


int main() {
	try{
		char userChoice = '0';
		string brokerage_name;
		string filename = "";
		
		cout << "Setting Name of Brokerage..." << endl;
		cout << "Enter Brokerage name as a string (no spaces): ";
		cin >> brokerage_name;
		cout << "Enter input File Name as a string with no spaces or special characters (e.g. BrokerFile1, or Q to skip File read): ";
		cin >> filename;
		
		Brokerage broke(brokerage_name);
		
		if (filename == "Q"){
			cout << "Skipping input File read and instead initializing a new Brokerage state" << endl;
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
			filename += ".txt";
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
			broke = Brokerage(name, vec_patron, vec_trans, cash, vec_shares);
		}

		while (userChoice != 'K') {
			try {
				cout << "Enter letter of desired option: " << endl;
				cout << "A: display information about Stock Shares & Cash in the Brokerage" << endl;
				cout << "B: add a new Patron to the Brokerage" << endl;
				cout << "C: check if someone is already a Patron" << endl;
				cout << "D: display all Patrons" << endl;
				cout << "E: add Cash to a Patron account" << endl;
				cout << "F: remove Cash from a Patron account" << endl;
				cout << "G: do a Stock Shares Sale for a Patron" << endl;
				cout << "H: do a Stock Shares Buy for a Patron" << endl;
				cout << "I: display all Patrons that are overdrawn" << endl;
				cout << "J: display all transactions ever done by Patrons" << endl;
				cout << "K: quit" << endl;
				cin >> userChoice;
				if (!cin) error("That isn't valid type of input.");
				
				switch (userChoice) {
					//display information about Stock shares and cash
					case 'A':
						char another_choice;
						cout << "Allowing you to view the state of the brokerage..." << endl;
						cout << "Enter Shares Stock Type (i=Intel, g=Google, a=Apple, y=Yahoo, I=IBM, or C=Cash or A=ALL): " << endl;
						cin >> another_choice;
						if (!cin) error("That isn't a valid type of input.");
						
						switch(another_choice){
							//choosing whether want to see stock type, cash, or everything
							case 'i':
								cout << "Current INTC in Brokerage: ";
								cout << broke.shares_info(0) << endl;
								break;
							case 'g':
								cout << "Current GOOG in Brokerage: ";
								cout << broke.shares_info(1) << endl;
								break;
							case 'a':
								cout << "Current AAPL in Brokerage: ";
								cout << broke.shares_info(2) << endl;
								break;
							case 'y':
								cout << "Current YHOO in Brokerage: ";
								cout << broke.shares_info(3) << endl;
								break;
							case 'I':
								cout << "Current IBM in Brokerage: ";
								cout << broke.shares_info(4) << endl;
								break;
							case 'C':
								cout << "Current Cash in Brokerage: " << fixed << setprecision(2) << broke.cash() << endl;
								break;
							case 'A':
								cout << "Current INTC in Brokerage: ";
								cout << broke.shares_info(0) << endl;
								cout << "Current GOOG in Brokerage: ";
								cout << broke.shares_info(1) << endl;
								cout << "Current AAPL in Brokerage: ";
								cout << broke.shares_info(2) << endl;
								cout << "Current YHOO in Brokerage: ";
								cout << broke.shares_info(3) << endl;
								cout << "Current IBM in Brokerage: ";
								cout << broke.shares_info(4) << endl;
								cout << "Current Cash in Brokerage: " << fixed << setprecision(2) << broke.cash() << endl;
								cout << "Current USDs in Brokerage: " << fixed << setprecision(2) << broke.USD() << endl;
								break;
							default:
								error("You didn't enter one of the options specified.");
								break;
						}
					break;
					
					//add a new Patron to Brokerage
					case 'B':{
						string patron_name;
						int acct_num;
						double initial_deposit;
					
						cout << "Adding Patron to the Brokerage..." << endl;
						
						cout << "Enter Patron's name (use underscores for spaces): ";
						cin >> patron_name;
						
						cout << "Enter Patron's Brokerage accout number (a nonnegative integer): ";
						cin >> acct_num;
						if (!cin) error("That isn't the valid type of input.");
						if (acct_num < 0) error("You can't have a negative account number.");
						
						//Checking if the account number has already been taken by another patron
						if (broke.find_patron(acct_num) != -1) error("This account number has already been taken.");
						
						cout << "Enter intitial deposit in USD's for this Patron: ";
						cin >> initial_deposit;
						if (!cin) error("That isn't the valid type of input.");
						if (initial_deposit < 0) error("You can't have a negative deposit.");
						
						broke.add_cash(initial_deposit); //Add cash to brokerage as well

						Patron p(patron_name, acct_num, initial_deposit);
						broke.addPatron(p);
					}
					break;
					
					//check if someone is already a Patron
					case 'C':
						{
							int brokerage_number = 0;
							int i = 0;
							cout << "Searching for Patron in Brokerage... " << endl;
							cout << "Enter Patron's Brokerage account number (a nonnegative integer): ";
							cin >> brokerage_number;
							if (!cin) error("That isn't the valid type of input.");
							if (brokerage_number < 0) error("You can't have a negative account number.");

							for (i = 0; i < broke.patron_size(); ++i) {
								if (brokerage_number == broke.patron(i).number()) {
									cout << "Yes, there is a Patron " << broke.patron(i).name() << " with that brokerage account number." << endl;
									cout << broke.patron(i) << endl;
									break;
								}
								if (i == (broke.patron_size() - 1) && brokerage_number != broke.patron(i).number()) {	
									cout << "No, there is not a Patron with that Brokerage account number." << endl;
								}
							}
						break;
						}
					
					//display all patrons
					case 'D':
						cout << "Displaying All Brokerage Patrons..." << endl;
						for (int i = 0; i < broke.patron_size(); i++){
							cout << broke.patron(i) << endl;
						}
					break;
					
					//add Cash to a patron account
					case 'E':
						{
						cout << "Adding Cash to Brokerage for Patron..." << endl;
						cout << "Enter Brokerage account number of Patron doing deposit: ";
					
						//Chooses the number the user will compare the number of the account to
						int comp_number;
						cin >> comp_number;
						if (!cin) error("That isn't the valid type of input.");
						if (comp_number < 0) error("You can't have a negative account number.");
						
						int index = broke.find_patron(comp_number);
						if (index == -1) error("There is no patron by this number.");
						
						cout << "Enter deposit in USD's for this Patron: ";
						
						//Chooses the cash amount they wish to add
						double new_cash;
						cin >> new_cash;
						if (!cin) error("That isn't the valid type of input.");
						if (new_cash < 0) error("You can't have a negative deposit.");
						
						cout << fixed << setprecision(2) << "Adding Cash " << new_cash << " to Patron " << broke.patron(index).name() << " account." << endl;
						
						double old_cash = broke.patron(index).cash();
						
						cout << "Old Account Cash Balance: " << old_cash << endl;
						cout << "Old Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						broke.patron(index).add_cash(new_cash);
						broke.add_cash(new_cash);
						cout << "New Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "New Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						cout << "Updated Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "Updated Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						//Adding the Transaction
						Time t;
						Date d;
							broke.addTransaction(Transaction(broke.patron(index),'c',"Cash_Deposit",new_cash,t,d));
						}
						
					break;
					
					//remove Cash from a patron account
					case 'F':
						{
						cout << "Removing Cash from Brokerage for Patron..." << endl;
						cout << "Enter Brokerage account number of Patron doing deposit: ";
					
						//Chooses the number the user will compare the number of the account to
						int acct_number;
						cin >> acct_number;
						if (!cin) error("That isn't the valid type of input.");
						if (acct_number < 0) error("You can't have a negative account number.");
						
						int index = broke.find_patron(acct_number);
						if (index == -1) error("There is no patron by this number.");
						
						cout << "Enter withdrawl in USD's for this Patron: ";
						
						//Chooses the cash amount they wish to add
						double new_cash;
						cin >> new_cash;
						if (!cin) error("That isn't the valid type of input.");
						if (new_cash < 0) error("You can't have a negative deposit.");
						if (new_cash > broke.patron(index).cash()) error("You can't remove cash that you don't have");
						
						cout << fixed << setprecision(2) << "Removing Cash " << new_cash << " to Patron " << broke.patron(index).name() << " account." << endl;
						
						double old_cash = broke.patron(index).cash();
						
						cout << "Old Account Cash Balance: " << old_cash << endl;
						cout << "Old Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						broke.patron(index).remove_cash(new_cash);
						broke.remove_cash(new_cash);
						cout << "New Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "New Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						cout << "Updated Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "Updated Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						Time t;
						Date d;
						broke.addTransaction(Transaction(broke.patron(index),'c',"Cash_Withdrawl",new_cash,t,d));
						}
					break;
					
					//do a Stock shares sale for a patron
					case 'G':
						{
						int acct_num = 0;
		        cout << "Adding Cash to Brokerage for Patron Stock Shares sell..." << endl;
						cout << "Enter Brokerage account number of Patron doing sell: ";
						cin >> acct_num;
						if (!cin) error("That isn't the valid type of input.");
						if (acct_num < 0) error("You can't have a negative account number.");
						
						int index = broke.find_patron(acct_num);
						if (index == -1) error("There is not patron with this account number."); //throws an error if there isn't a account number

						char stock_type;
						cout << "Enter Shares Stock Type (i = Intel, g = Google, a = Apple, y = Yahoo, I = IBM): ";
						cin >> stock_type;
						if (!cin) error("That isn't the valid type of input.");

						int stock_amount;
						cout << "Enter Shares Stock Amount: ";
						cin >> stock_amount;
						if (!cin) error("That isn't the valid type of input.");
						
						int stock_number; //index number associated with the type

//FIXME Not all Stocks Included
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
								error("That wasn't a valid option");
								break;
						}
						
						//Getting the number of patron shares and checking it
						
						double original_cash = broke.patron(index).cash();
						double original_value = broke.patron(index).total_value();
						
						double patron_total_shares = broke.patron(index).num_shares(stock_number);
						if (patron_total_shares < stock_amount){
							error("You can't sell shares you don't have");
						}
						
						//Actually implementing actually changin the number of shares
						double broke_total_shares = broke.shares_info(stock_number).number();
						cout << "Old " << broke.shares_info(stock_number).type().name() << " in Brokerage: " << broke.shares_info(stock_number) << endl;
						broke.patron(index).update_shares(stock_number, patron_total_shares - stock_amount); //updating the shares in the patron account
						broke.shares_info(stock_number).update(broke_total_shares - stock_amount); //updating the shares in the brokerage
						cout << "New " << broke.shares_info(stock_number).type().name() << " in Brokerage: " << broke.shares_info(stock_number) << endl; //updating the 
						cout << "Adding Cash " << (stock_amount * broke.shares_info(stock_number).type().price()) << " to Patron " << broke.patron(index).name() << " account." << endl;
						broke.patron(index).add_cash((stock_amount * broke.shares_info(stock_number).type().price()));
						broke.add_cash(stock_amount * broke.shares_info(stock_number).type().price());
						cout << "Old Account Cash Balance: " << original_cash << endl;
						cout << "Old Account USDs Balance: " << original_value << endl;
						cout << "New Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "New Account USDs Balance: " << broke.patron(index).total_value() << endl;
						cout << "Updated Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "Updated Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						//Transaction part to keep tabs on thes transactions
						Time t;
						Date d;
						broke.addTransaction(Transaction(broke.patron(index),stock_number,"Stock_Sale",stock_amount,t,d));
						break;
					}
					
					//do a stock shares buy for a patron
					case 'H':
						{
						int acct_num = 0;
		        cout << "Removing Cash from Brokerage for Patron Stock Shares buy..." << endl;
						cout << "Enter Brokerage account number of Patron doing buy: ";
						cin >> acct_num;
						if (!cin) error("That isn't the valid type of input.");
						if (acct_num < 0) error("You can't have a negative account number.");
						
						int index = broke.find_patron(acct_num);
						if (index == -1) error("There is not patron with this account number."); //throws an error if there isn't a account number

						char stock_type;
						cout << "Enter Shares Stock Type (i = Intel, g = Google, a = Apple, y = Yahoo, I = IBM): ";
						cin >> stock_type;
						if (!cin) error("That isn't the valid type of input.");

						int stock_amount;
						cout << "Enter Shares Stock Amount: ";
						cin >> stock_amount;
						int stock_number; //index number associated with the type

						//FIXME Not all Stocks Included
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
								error("That wasn't a valid option");
								break;
						}
						
						//Getting the number of patron shares and checking it
						double original_cash = broke.patron(index).cash();
						double original_value = broke.patron(index).total_value();
						double patron_total_shares = broke.patron(index).num_shares(stock_number);
						
						//Actually implementing actually changin the number of shares
						double broke_total_shares = broke.shares_info(stock_number).number();
						cout << "Old " << broke.shares_info(stock_number).type().name() << " in Brokerage: " << broke.shares_info(stock_number) << endl;
						broke.patron(index).update_shares(stock_number, patron_total_shares + stock_amount); //updating the shares in a patron's account
						broke.shares_info(stock_number).update(broke_total_shares + stock_amount); //updating the shares in the brokerage
						broke.patron(index).remove_cash(stock_amount * broke.shares_info(stock_number).type().price()); //updating the cash in the patron's account
						broke.remove_cash(stock_amount * broke.shares_info(stock_number).type().price()); //updating the cash in the brokerage
						cout << "New " << broke.shares_info(stock_number).type().name() << " in Brokerage: " << broke.shares_info(stock_number) << endl;
						cout << "Removing Cash " << (stock_amount * broke.shares_info(stock_number).type().price()) << " from Patron " << broke.patron(index).name() << " account." << endl;
						cout << "Old Account Cash Balance: " << original_cash << endl;
						cout << "Old Account USDs Balance: " << original_value << endl;
						cout << "New Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "New Account USDs Balance: " << broke.patron(index).total_value() << endl;
						cout << "Updated Account Cash Balance: " << broke.patron(index).cash() << endl;
						cout << "Updated Account USDs Balance: " << broke.patron(index).total_value() << endl;
						
						
						Time t;
						Date d;
						broke.addTransaction(Transaction(broke.patron(index),stock_number,"Stock_Buy",stock_amount,t,d));
						break;
						}
					
					//display all overdrawn patrons
					case 'I':
						cout << "Displaying All Overdrawn Brokerage Patrons..." << endl;
						for (int i = 0; i < broke.patron_size(); i++){
							if (broke.patron(i).cash() < 0)
								cout << broke.patron(i) << endl;
						}
					break;
					
					//display all transactions ever done by patrons
					case 'J':
						cout << "Displaying All Brokerage Transactions..." << endl;
						for (int i = 0; i < broke.transactions_size(); i++){
							cout << broke.transaction(i) << endl;
						}
					break;
					
					case 'K':
						//This does nothing
					break;

					// error catching
					default:
						error("You didn't enter a menu choice. Please try again.");
					break;
				}
			}
			catch (runtime_error e){
				cerr << e.what() << endl;
				cin.clear(); // clears the error state
 	 			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //flushes the buffer
			}
			catch (...) {
				cerr << "There is an unknown error. Please try again" << endl;
			}	
		}
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
			output_filename += ".txt";
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
	catch (Corrupted_File) {
			cerr << "The file you have provided is corrupted. Exiting the program.";
	}
	catch (Filename_error){
		
	}
	return 0;
}