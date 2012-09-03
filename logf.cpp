#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <stdlib.h>

#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QStack>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char * thispath() {								//
										//
	int MAXPATHLENGTH = 250;						//
										//
	char * _path = (char *)malloc(sizeof(char *) * MAXPATHLENGTH);		// GET THE DIRECTORY THAT INVOKED THE COMMAND
	getcwd(_path, MAXPATHLENGTH);						//
	return _path;								// 
										//
}										//
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
void print_bit(QFileInfo &, QString, QString);			// HANDLE FILE CONTENTS
//////////////////////////////////////////////////////////////////

void printbit_php(QFileInfo & _file, QString searchword, QString stopword);
void handle_param(QString & path, int argc, char* argv[]);
void handle_single_file();
void print_until(QStringList & list, QString stop);
QString return_message(QStringList & list);

#define PARAM_IS_NULL 0
#define PARAM_IS_DIR 1
#define PARAM_IS_FILE 2
#define PARAM_IS_BROKEN 3

// GLOBALS ///////////////////////////////////////////////////////
int STATUS = PARAM_IS_NULL;					//
QString * FULLPATH = NULL;					//
QStringList * ALLOWED_TYPES = NULL;				//
//////////////////////////////////////////////////////////////////

void handle_param(QString & path, int argc, char* argv[]) 
{

	char * _param;
	//////////////////////////////////////////////////////////////////
	if(argc >= 1) {							//
		_param = argv[1];					//
	} else {							// CHECK FOR PARAMS
		_param = NULL;						//
	}								//
	//////////////////////////////////////////////////////////////////


	QString param(_param);
	FULLPATH = new QString(path + "/" + _param);

	QFile   param_file ( *FULLPATH );
	QDir    param_dir  ( *FULLPATH );

	if( param_dir.exists() ) {

		STATUS = PARAM_IS_DIR;

	} else if ( param_file.exists() ) {

		STATUS = PARAM_IS_FILE;

	} else {

		STATUS = PARAM_IS_BROKEN;

	}

}

void handle_single_file() {

	QFileInfo file_info(*FULLPATH);
	
	QString ext = file_info.suffix();

	if(ALLOWED_TYPES->contains(ext)) {

		if(ext == "php") {
			printbit_php(file_info, "function", "{");
		}		

	} else {
		return;
	}	

}

int main(int argc, char* argv[])
{	

	///////////////////////////////////
	QString path = thispath();	 // GET PATH
	///////////////////////////////////


	ALLOWED_TYPES = new QStringList();
	ALLOWED_TYPES->append("php");


	handle_param(path, argc, argv);

	switch(STATUS) {
	
		case PARAM_IS_BROKEN:
			std::cout << "Bad argument.\nAborted...\n";
			break;
		case PARAM_IS_FILE:
			handle_single_file();
			break;

	}
	

}

void print_bit (QFileInfo & _file, QString searchword, QString stopword) 
{

	std::cout << "*" <<  _file.absoluteFilePath().toStdString() << "\n";
	
	//////////////////////////////////////////
	QFile file(_file.absoluteFilePath());	// FILE THAT WILL BE READ FROM
	//////////////////////////////////////////
	
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		
	}	


}

void printbit_php(QFileInfo & _file, QString searchword, QString stopword) 
{

	QFile file(_file.absoluteFilePath());

	if( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {

		QTextStream in(&file);

		///////////////////////////////////////////////////////////////
		std::cout << "*" << _file.absoluteFilePath().toStdString() << "\n";
		///////////////////////////////////////////////////////////////
		QStack<char> closers;

		QString return_buffer;

		while( ! in.atEnd() ) {
			
			QString line = in.readLine();
			QStringList line_list = line.split(" ");
			line_list.replaceInStrings(" ", "");
			line_list.replaceInStrings("\n", "");
			line_list.replaceInStrings("\t", "");

			if(line_list.indexOf("class") != -1 && line_list.indexOf("{") != -1) {
				closers.push('{');
				std::cout << "\n   >> ";
				print_until(line_list, "{");
				std::cout << "\n";
				continue;
			}

			int open  = line.count("{");
			int close = line.count("}");

			for(int i = 0; i < open; i++) closers.push('{');
			for(int i = 0; i < close; i++) closers.pop();

			if(line_list.indexOf("@return") != -1 || line_list.indexOf("//@return") != -1) {
				return_buffer.append(return_message(line_list));
			}

			if(line_list.indexOf("function") != -1) {
				std::cout << "\t";
				if(!closers.isEmpty()) std::cout << "   * "; 
				print_until(line_list, "{");

				if( ! return_buffer.isEmpty() ) {
					std::cout << return_buffer.toStdString();
					return_buffer = "";
				}			

				std::cout << "\n";
			}

			
			

		}

	}

}


void print_until(QStringList & list, QString stop) {

	for(int i = 0; i < list.size(); i++) {
		if(list.at(i) != stop) {
			if(list.at(i) == "function") {
			}

			std::cout << list.at(i).toStdString() << " ";

		}
	}

}


QString return_message(QStringList & list) {

	QString get("");
	

	int indice;
	if(list.at(0) == "//@return") {
		get.append("@return ");
		indice = 1;
	} else if(list.at(0) == "//") {
		get.append("@return ");
		indice = 2;
	}


	for(int i = indice; i < list.size(); i++) {
		get += list.at(i) + " ";
	}

	return get;

}
