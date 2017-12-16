#include "passwordWorker.h"
#include <KPasswordDialog>
void PasswordWorker::promptPassword(){
	QWidget * passwordWidget = new QWidget;
	KPasswordDialog dlg(passwordWidget);
dlg.setPrompt("Enter your password");
if( !dlg.exec() )
    return ; //the user canceled
this->password=dlg.password();
}
