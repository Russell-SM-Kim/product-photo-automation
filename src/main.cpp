#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

int main(
    int argc,
    char *argv[]
)
{
    QApplication app(
        argc,
        argv
    );

    QApplication::setApplicationName(
        "상품 촬영 자동화"
    );

    QApplication::setWindowIcon(
        QIcon(
            ":/assets/app_icon.png"
        )
    );

    MainWindow window;

    window.show();

    return app.exec();
}