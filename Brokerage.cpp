#include "Brokerage.h"

using namespace Chrono;

Stock::Stock()
	{
		
	}

Stock::Stock(string n, double p) :
	stock_name(n), stock_price(p)
	{
		
	}
	
Shares::Shares()
	{
		
	}
	
Shares::Shares(Stock s, double n)
	: stock_type(s), num_shares(n)
	{
		
	}
	
Patron::Patron(){}
	
Patron::Patron(string n, int a, double c)
	: patron_name(n), acct_num(a), cash_value(c)
	{
		Stock Intel("INTC", 35.91);
		Stock Google("GOOG", 830.62);
		Stock Apple("AAPL", 138.96);
		Stock Yahoo("YHOO", 45.94);
		Stock IBM("IBM", 180.53);
		
		all_shares = {Shares(Intel,0), Shares(Google,0), Shares(Apple,0), Shares(Yahoo,0), Shares(IBM,0)};
	}
	
Patron::Patron(string n, int a, double c, vector<Shares> s) :
	patron_name(n), acct_num(a), cash_value(c), all_shares(s)
	{
		
	}
	
Transaction::Transaction()
	{
	
	}

Transaction::Transaction(Patron p, char m, string t, double a, Time ti, Date d)
	: trans_p(p), money_type(m), trans_type(t), amount(a), trans_date(d), trans_time(ti)
	{
		Stock Intel("INTC", 35.91);
		Stock Google("GOOG", 830.62);
		Stock Apple("AAPL", 138.96);
		Stock Yahoo("YHOO", 45.94);
		Stock IBM("IBM", 180.53);
		
		vector<Stock> all_stocks = {Intel, Google, Apple, Yahoo, IBM};
		
		if (money_type =='c'){
			trans_type = trans_type + " CASH" ;
			monetary_value = 1;
		}
		else{
			trans_type = trans_type + " " + all_stocks.at(money_type).name();
			monetary_value = all_stocks.at(money_type).price();
		}
	}


Brokerage::Brokerage(string n, vector<Patron> p, vector<Transaction> trans, double c, vector<Shares> s) :
	broke_name(n), all_patrons(p), all_transactions(trans), total_cash(c), all_shares(s)
	{
	}

Brokerage::Brokerage(string n) : broke_name(n)
	{
		all_patrons = {};
		all_transactions = {};
		total_cash = 0;
		
		Stock Intel("INTC", 35.91);
		Stock Google("GOOG", 830.62);
		Stock Apple("AAPL", 138.96);
		Stock Yahoo("YHOO", 45.94);
		Stock IBM("IBM", 180.53);
		
		all_shares = {Shares(Intel,0), Shares(Google,0), Shares(Apple,0), Shares(Yahoo,0), Shares(IBM,0)};
	}
	
double Patron::total_value() const
	{
		double value = 0;
		for (int i = 0; i < 5; i++){
			value += all_shares.at(i).value();
		}
		value += cash_value;
		return value;
	}
	
int Brokerage::find_patron(int acct) const
	{
		for (int i = 0; i < all_patrons.size(); i++){
			if (all_patrons.at(i).number() == acct) return i;
		}
		return -1; //returns -1 if there isn't a patron by this number
	}
	
ostream& operator<<(ostream& os, const Stock& st)
{
	return os << st.name() << " " << st.price(); //writes the stock name and the stock price to the output stream
}

ostream& operator<<(ostream& os, const Shares& sh)
{
	return os << fixed << setprecision(2) << sh.type() << " " << sh.number() << " " << sh.value();
}

ostream& operator<<(ostream& os, const Patron& p)
{
	return os << fixed << setprecision(2) << p.name() << " " << p.number() << " " << p.cash() << " " << p.total_value() << " " 
		<< p.shares().at(0) << " " << p.shares().at(1) << " " << p.shares().at(2) << " " << p.shares().at(3) << " " << p.shares().at(4);
}

ostream& operator<<(ostream& os, const Transaction& t)
{
	return os << fixed << setprecision(2) << t.trans_p << " " << t.trans_time << " " << t.trans_date << " " << t.trans_type << " " << t.monetary_value << " " << t.amount << " " << t.monetary_value * t.amount; //FIXME
}

ostream& operator<<(ostream& os, const Brokerage& b)
{
	return os << fixed << setprecision(2) << "B " << b.name() << endl;
}

istream& operator>>(istream& is, Stock& st)
{
	string name;
	double price;
	is >> name >> price;
	//if (!is) throw Corrupted_File();
	st = Stock(name,price);
	return is;
}

istream& operator>>(istream& is, Shares& sh)
{
	Stock stock_name;
	double amount;
	string dump;
	is >> stock_name >> amount >> dump;
	//if (!is) throw Corrupted_File();
	sh = Shares(stock_name,amount);	
	return is;
}

istream& operator>>(istream& is, Patron& p)
{
	string name;
	int acct_num;
	double cash;
	double value;
	Shares shares_name;
	vector<Shares> total_shares;
	is >> name;
	is >> acct_num;
	is >> cash;
	is >> value;
	for (int i = 0; i < 5; ++i)	{
		is >> shares_name;
		total_shares.push_back(shares_name);
	}
	//if (!is) throw Corrupted_File();
	p = Patron(name, acct_num, cash, total_shares);
	return is;
}

istream& operator>>(istream& is, Transaction& t)
{
	Date d;
	Time ti;
	Patron p;
	string trans_type;
	string money_type;
	char mon_type;
	int stock_type;
	double amount;
	double trash;
	double trash2;
	is >> p >> ti >> d >> trans_type >> money_type >> trash >> amount >> trash2;
	//if (!is) throw Corrupted_File();
	if (money_type == "CASH"){
		mon_type = 'c';
		t = Transaction(p, mon_type, trans_type, amount, ti, d);
	}
	else{
		if (money_type == "INTC") stock_type = 0;
		if (money_type == "GOOG") stock_type = 1;
		if (money_type == "AAPL") stock_type = 2;
		if (money_type == "YHOO") stock_type = 3;
		if (money_type == "IBM") stock_type = 4;
		t = Transaction(p, stock_type, trans_type, amount, ti, d);
	}
	return is;
}