#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect (ui->txtParserInput, SIGNAL(textChanged ()), this, SLOT(onTextChanged ()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged () {
    using namespace Sql::Exceptions;

    const QString source = ui->txtParserInput->toPlainText ();
    parser.reset ();
    QString result = "Accepted!";


    try {
        parser.feed (source.toStdString ().c_str ());
    } catch (IllegalTokenException) {
        result = "Illegal token in input stream";
    } catch (IllegalModificationException) {
        result = "Illegal modification of token stream";
    }

    try {
        parser.parse ();
    } catch (UnknownSubsetException &ex) {
        result = "Unkown subset in line ";
        result.append (QString::number (ex.getLine ()));
    } catch (EndOfStreamException) {
        result = "Unexpected end of input stream";
    } catch (StreamNotEmptyException) {
        result = "Tokens left at the end of parsing";
    } catch (IllegalModificationException) {
        result = "Illegal modification of token stream";
    } catch (IllegalParserStateException) {
        result = "Illegal parser state";
    } catch (IllegalTokenException) {
        result = "Illegal token in input stream";
    } catch (UnexpectedTokenException &ex) {
        result = "Unexpected token. Found '";
        result.append (ex.getFound ().c_str ());
        result.append ("' but expected one of ");
        for (const std::string& item: ex.getExpected ()) {
            result.append (item.c_str ());
            result.append (" ");
        }
    }

    ui->lblParserOutput->setText (result);

    std::stringstream ss;
    parser.printSyntaxTree (ss);

    ui->txtResult->setPlainText (QString::fromStdString (ss.str ()));

}
