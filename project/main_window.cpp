#include "main_window.h"
#include <sstream>
#include <string>
#include <QComboBox> 
#include <QMessageBox> 
#include <QFileDialog>
#include <fstream>
#include <cstdlib> 
using namespace std;

struct alphaComp {
    bool operator()(const Product* lhs, const Product* rhs) 
    { 
     	return lhs->getName() < rhs->getName(); 
   	}
};
struct ratingComp {
	MyDataStore* ds;
	ratingComp(MyDataStore* datastore):ds(datastore){}
    bool operator()(const Product* lhs, const Product* rhs) 
    { 
    	
      	return (ds->getRating(lhs->getName())) > (ds->getRating(rhs->getName())); // > from high to low
    	
    }
};

struct dateComp {
    bool operator()(const Review* lhs, const Review* rhs) 
    { 
     	return lhs->date > rhs->date; // > from most recent to least recent
   	}
};
MainWindow::MainWindow(MyDataStore* database): ds(database)
{
	setWindowTitle("Amazon");

	overallLayout=new QHBoxLayout();

	

	//box on the left with search and user swtich
	formLayout=new QVBoxLayout();
	overallLayout->addLayout(formLayout);

	//keyword input box
	keywordLabel=new QLabel("keywords:");
	formLayout->addWidget(keywordLabel);

	keywordInput=new QLineEdit();
	formLayout->addWidget(keywordInput);

	//and search button
	ANDsearchButton=new QPushButton("AND");
	connect(ANDsearchButton,SIGNAL(clicked()),this,SLOT(ANDsearch()));
	formLayout->addWidget(ANDsearchButton);

	//or search button
	ORsearchButton=new QPushButton("OR");
	connect(ORsearchButton,SIGNAL(clicked()),this,SLOT(ORsearch()));
	formLayout->addWidget(ORsearchButton);

	//switch user combobox label
	userSwitch=new QLabel("switch user:");
	formLayout->addWidget(userSwitch);

	//switch user combobox
	userComboBox=new QComboBox();
	std::vector<string> userNames=ds->userNameList();
	for(unsigned int i=0;i<userNames.size();i++)
	{
		userComboBox->addItem(QString::fromStdString(userNames[i]));
	}
	connect(userComboBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(switchUser(const QString &)));
	formLayout->addWidget(userComboBox);

	//line input for username 
	userNameInput=new QLineEdit();
	formLayout->addWidget(userNameInput);
	connect(userNameInput,SIGNAL(returnPressed()),this,SLOT(switchUser()));

	//connect(userNameInput,SIGNAL(returnPressed()),userComboBox,SLOT(setCurrentText(const QString &)));


	//Qlabel display current user
	userName=userNames[0];
	displayUser=new QLabel("User: "+QString::fromStdString(userNames[0]));
	formLayout->addWidget(displayUser);
	setLayout(overallLayout);

	//quit button
	quitButton=new QPushButton("quit");
	formLayout->addWidget(quitButton);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(quit()));


	//at middle, list display of search result 
	listLayout=new QVBoxLayout();
	overallLayout->addLayout(listLayout);

	searchMethodSwitch=new QComboBox();
	searchMethodSwitch->addItem("alpha");
	searchMethodSwitch->addItem("rating");
	connect(searchMethodSwitch,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(switchSearch(const QString &)));
	listLayout->addWidget(searchMethodSwitch);

	searchListWidget=new QListWidget();
	listLayout->addWidget(searchListWidget);

	//add cart button
	addCartButton=new QPushButton("Add to Cart");
	connect(addCartButton,SIGNAL(clicked()),this,SLOT(addCart()));
	listLayout->addWidget(addCartButton);

	//view cart button
	viewCartButton=new QPushButton("View Cart");
	connect(viewCartButton,SIGNAL(clicked()),this,SLOT(viewCart()));
	listLayout->addWidget(viewCartButton);


	//at right, display reviews of selected product
	reviewLayout=new QVBoxLayout;
	overallLayout->addLayout(reviewLayout);

	reviewsLabel=new QLabel("reviews:");
	reviewLayout->addWidget(reviewsLabel);

	reviewsList=new QListWidget;
	reviewLayout->addWidget(reviewsList);
	connect(searchListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(displayReview(int)));

	addReviewButton=new QPushButton("Add review");
	connect(addReviewButton,SIGNAL(clicked()),this,SLOT(addReview()));
	reviewLayout->addWidget(addReviewButton);






	//second window
	otherWin=new QWidget;

	cartList=new QListWidget;


	buyButton=new QPushButton("Buy Cart");
	removeButton=new QPushButton("remove selected item");
	closeButton=new QPushButton("close");

	otherWinLayout=new QVBoxLayout;
	otherWinLayout->addWidget(cartList);
	otherWinLayout->addWidget(buyButton);
	otherWinLayout->addWidget(removeButton);
	otherWinLayout->addWidget(closeButton);

	otherWin->setLayout(otherWinLayout);


	connect(buyButton,SIGNAL(clicked()),this,SLOT(buyCart()));
	connect(removeButton,SIGNAL(clicked()),this,SLOT(removeSelected()));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(otherWinClose()));



	//review window
	reviewWin=new QWidget;
	reviewWinLayout=new QVBoxLayout;
	dateLayout=new QHBoxLayout;

	ratingLabel=new QLabel("rating:");
	dateLabel=new QLabel("date:");
	textLabel=new QLabel("text:");

	reviewWinLayout->addWidget(ratingLabel);
	rating=new QComboBox;
	for (int i=1;i<=5;i++)
	{
		rating->addItem(QString::number(i,10));
	}
	reviewWinLayout->addWidget(rating);

	reviewWinLayout->addWidget(dateLabel);
	year=new QComboBox;
	for (int i=2016;i<=2030;i++)
	{
		year->addItem(QString::number(i,10));
	}
	dateLayout->addWidget(year);

	month=new QComboBox;
	for (int i=1;i<=12;i++)
	{
		month->addItem(QString::number(i,10));
	}
	dateLayout->addWidget(month);	

	day=new QComboBox;
	for (int i=1;i<=31;i++)
	{
		day->addItem(QString::number(i,10));
	}
	dateLayout->addWidget(day);
	reviewWinLayout->addLayout(dateLayout);



	reviewWinLayout->addWidget(textLabel);
	reviewText=new QLineEdit;
	reviewWinLayout->addWidget(reviewText);

	reviewEnterButton=new QPushButton("OK");
	reviewWinLayout->addWidget(reviewEnterButton);
	connect(reviewEnterButton,SIGNAL(clicked()),this,SLOT(enterReview()));




	
	
	reviewWin->setLayout(reviewWinLayout);
	

}




MainWindow::~MainWindow() 
{	
	
}

void MainWindow::ANDsearch()
{
	searchListWidget->clear();
	string term;
    std::vector<string> terms;
    stringstream ss(keywordInput->text().toStdString());

    while(ss >> term) {
        term = convToLower(term);
        terms.push_back(term);
    }
    hits = ds->search(terms, 0);

    if(searchMethodSwitch->currentText()=="alpha")
    {
    	
    	mergesort(hits,alphaComp());
    }
    else
    {
    	ratingComp comp(ds);
    	mergesort(hits,comp);
    }
    for(unsigned int i=0;i<hits.size();i++)
    {
    	stringstream info;
    	info<<hits[i]->getName()<<' '<<hits[i]->getPrice();
    	searchListWidget->addItem(QString::fromStdString((info.str())));
    	
    }
	keywordInput->setText("");
}

void MainWindow::ORsearch()
{
	searchListWidget->clear();
	string term;
    std::vector<string> terms;
    stringstream ss(keywordInput->text().toStdString());

    while(ss >> term) {
        term = convToLower(term);
        terms.push_back(term);
    }
    hits = ds->search(terms, 1);
   
    if(searchMethodSwitch->currentText()=="alpha")
    {
    	mergesort(hits,alphaComp());
    }
    else
    {
    	ratingComp comp(ds);
    	mergesort(hits,comp);
    }
    for(unsigned int i=0;i<hits.size();i++)
    {
    	stringstream info;
    	info<<hits[i]->getName()<<' '<<hits[i]->getPrice();
    	searchListWidget->addItem(QString::fromStdString((info.str())));
    }
    keywordInput->setText("");
	
}

void MainWindow::switchUser(const QString & userNameSelected)
{
	userName=userNameSelected.toStdString();
	displayUser->setText("User: "+userNameSelected);
	displayCart();
	
}
void MainWindow::switchUser()
{
	//check if correct userName
	int index=userComboBox->findText(userNameInput->text());
	if(index<0)
		return;
	userName=userNameInput->text().toStdString();
	displayUser->setText("User: "+userNameInput->text());
	userNameInput->clear();
	userComboBox->setCurrentIndex(index);
}

void MainWindow::switchSearch(const QString & s)
{
	
	if(s== "alpha")
    {
    	mergesort(hits,alphaComp());
    }
    else
    {
    	ratingComp comp(ds);
    	mergesort(hits,comp);
    }
    searchListWidget->clear();
  
    for(unsigned int i=0;i<hits.size();i++)
    {

    	stringstream info;
    	info<<hits[i]->getName()<<' '<<hits[i]->getPrice();
    	searchListWidget->addItem(QString::fromStdString((info.str())));
    }

}

void MainWindow::addCart()
{
	ds->addCart(userName,searchListWidget->currentRow()+1,hits);
	displayCart();
}

void MainWindow::viewCart()
{
	
	otherWin->show();
	
	displayCart();
}

void MainWindow::displayCart()
{
	cartList->clear();
	
	std::vector<std::string> names=ds->cartNameList(userName);
	
	for(unsigned int i=0;i<names.size();i++)
    {
    	cartList->addItem(QString::fromStdString(names[i]));
    }
}

void MainWindow::buyCart()
{
	ds->buyCart(userName);
	displayCart();
}

void MainWindow::removeSelected()
{
	ds->removeCartItem(cartList->currentRow(),userName);
	displayCart();
}

void MainWindow::otherWinClose()
{
	otherWin->hide();
}

void MainWindow::displayReview(int searchIndex)
{
	reviewsList->clear();
	if(searchIndex==-1)
		return;
	std::vector<Review*> reviews=ds->getReviews(hits[searchIndex]->getName());
	mergesort(reviews,dateComp());

	for(unsigned int i=0;i<reviews.size();i++)
	{
		stringstream ss;
		ss<<reviews[i]->rating<<' '<<reviews[i]->username<<' '<<reviews[i]->date<<' '<<reviews[i]->reviewText;
		reviewsList->addItem(QString::fromStdString(ss.str()));
	}
}

void MainWindow::quit()
{
	std::string filename=QFileDialog::getSaveFileName().toStdString();
	if (filename=="")
		return;
	ofstream ofile(filename.c_str());
    ds->dump(ofile);
   	QApplication::quit();
    
}

void MainWindow::addReview()
{
	if(searchListWidget->currentRow()<0)
		return;
	reviewWin->setWindowTitle("add review");
	reviewWin->show();
}


void MainWindow::enterReview()
{
	string prod=hits[searchListWidget->currentRow()]->getName();
	int rate=strtol(rating->currentText().toStdString().c_str(),0,10);

	string u=userName;
	string d=year->currentText().toStdString()+"-"+month->currentText().toStdString()+"-"+day->currentText().toStdString();
	string review_text=reviewText->text().toStdString();

	Review* added=new Review(prod,rate,u,d,review_text);
	ds->addReview(added);
	reviewText->clear();
	reviewWin->hide();
	displayReview(searchListWidget->currentRow());
}