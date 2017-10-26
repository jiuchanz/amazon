#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include "mydatastore.h"
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <QComboBox>
#include <QMainWindow>  
#include <QPushButton>  
#include <QLineEdit>  
#include <QLayout>  
#include <QLabel>  
#include <QComboBox>  
#include <QMessageBox>  
#include <QDialog>
#include "msort.h"  
#include <QDialogButtonBox>
#include <QCalendarWidget>
#include <QApplication>
class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(MyDataStore*);
	~MainWindow();

private slots:
	void ANDsearch();
	void ORsearch();
	void switchUser(const QString &);
	void switchUser();
	void switchSearch(const QString &);
	void viewCart();
	void addCart();
	void buyCart();
	void removeSelected();
	void otherWinClose();
	void displayReview(int);
	void quit();
	void addReview();
	void enterReview();
	

private:
	
	void displayCart();
	//overall Layouts
	QHBoxLayout* overallLayout;


	QVBoxLayout* listLayout;
	QComboBox* searchMethodSwitch;
	QListWidget* searchListWidget;
	QPushButton* addCartButton;
	QPushButton* viewCartButton;
	

	QVBoxLayout* formLayout;
	QLabel* keywordLabel;
	QLineEdit* keywordInput;

	QPushButton* ANDsearchButton;
	QPushButton* ORsearchButton;

	QLabel* userSwitch;
	QComboBox* userComboBox;
	QLineEdit* userNameInput;
	QLabel* displayUser;

	QVBoxLayout* reviewLayout;
	QLabel* reviewsLabel;
	QListWidget* reviewsList;
	QPushButton* addReviewButton;

	QWidget* otherWin;
	QListWidget* cartList;
	QVBoxLayout* otherWinLayout;
	QPushButton* buyButton;
	QPushButton* removeButton;
	QPushButton* closeButton;

	QPushButton* quitButton;

	QWidget* reviewWin;
	QLabel* ratingLabel;
	QComboBox* rating;
	QLabel* dateLabel;
	QComboBox* year;
	QComboBox* month;
	QComboBox* day;
	QLabel* textLabel;
	QLineEdit* reviewText;
	QPushButton* reviewEnterButton;
	QVBoxLayout* reviewWinLayout;
	QHBoxLayout* dateLayout;


	//datastore 
	MyDataStore* ds;

	//vector<Product*> hits;
	std::string userName;

	//search result
	std::vector<Product*> hits;

};
