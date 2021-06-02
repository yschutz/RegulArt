#include "regulpushbutton.h"

#include <QImageReader>

RegulPushButton::RegulPushButton(QWidget* parent) : QPushButton(parent)
{
    QString sheet("RegulPushButton{background-color: transparent;} RegulPushButton:hover{border: 1px solid red}");
    setStyleSheet(sheet);
}

void RegulPushButton::setImageFileName(const QString &fileName)
{
    m_imageFileName = fileName;
    QImageReader reader;
    reader.setFileName(fileName);
    reader.setAutoTransform(true);
    QPixmap pm = QPixmap::fromImageReader(&reader);

    setIcon(QIcon(pm));
    setIconSize(pm.rect().size());
}
