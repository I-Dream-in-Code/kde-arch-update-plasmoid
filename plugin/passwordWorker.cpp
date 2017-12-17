#include "passwordWorker.h"
#include <KPasswordDialog>
void PasswordWorker::promptPassword(bool konsoleFlag, bool aur)
{
	QWidget *passwordWidget = new QWidget;
	KPasswordDialog dlg(passwordWidget);
	dlg.setPrompt("Enter your password");

	if (!dlg.exec())
		return ; //the user canceled
	emit workerUpgradeSystemSignal(konsoleFlag,aur,dlg.password());
}

