/*********************************************************************************
NixNote - An open-source client for the Evernote service.
Copyright (C) 2015 Randy Baumgarte

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
***********************************************************************************/

#include "locationdialog.h"
#include "src/global.h"

#include <QGridLayout>
#include <QLabel>
#include <cmath>

extern Global global;

LocationDialog::LocationDialog(QWidget *parent) :
        QDialog(parent) {
    wasOkPressed = false;
    setWindowTitle(tr("Location"));
    QGridLayout *grid = new QGridLayout();
    QGridLayout *input = new QGridLayout();
    QGridLayout *button = new QGridLayout();
    setLayout(grid);

    input->addWidget(new QLabel(tr("Latitude"), this), 1, 1);
    input->addWidget(&latitude, 1, 2);

    input->addWidget(new QLabel(tr("Longitude"), this), 2, 1);
    input->addWidget(&longitude, 2, 2);

    input->addWidget(new QLabel(tr("Altitude (optional)"), this), 3, 1);
    input->addWidget(&altitude, 3, 2);
    input->setContentsMargins(10, 10, -10, -10);
    grid->addLayout(input, 1, 1);

    ok.setText(tr("OK"));
    ok.setDefault(true);
    connect(&ok, SIGNAL(clicked()), this, SLOT(okButtonPressed()));

    QPushButton *cancel = new QPushButton(tr("Cancel"), this);
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    button->addWidget(&ok, 1, 1);
    button->addWidget(cancel, 1, 2);
    grid->addLayout(button, 3, 1);

    // validators did not worked OK, so I removed them ~temporarily
    // longlatval.setBottom(-90.0);
    // longlatval.setTop(90.0);
    // longlatval.setNotation(QDoubleValidator::StandardNotation);
    //
    // altitudeval.setBottom(-1500);
    // altitudeval.setTop(9999.99);
    // altitudeval.setNotation(QDoubleValidator::StandardNotation);
    // longitude.setValidator(&longlatval);
    // latitude.setValidator(&longlatval);
    // QLOG_DEBUG() << "Initialized location validator with locale=" << longlatval.locale();
    // altitude.setValidator(&altitudeval);

    this->setFont(global.getGuiFont(font()));
}


// The OK button was pressed
void LocationDialog::okButtonPressed() {
    locationText();
    wasOkPressed = true;
    close();
}

// The CANCEL button was pressed
void LocationDialog::cancelButtonPressed() {
    wasOkPressed = false;
    close();
}


bool LocationDialog::okPressed() {
    return wasOkPressed;
}

QString LocationDialog::locationTextOLD() {
    const QString &latT = latitude.text();
    double lat = latT.toDouble();
    QString p1 = calculateDegrees(lat);
    QLOG_DEBUG() << "Got latitude " << latT << ", " << lat << ", degrees: " << p1;
    if (lat >= 0)
        p1 = p1 + " N ";
    else
        p1 = p1 + " S ";

    const QString &lonT = longitude.text();
    double lon = lonT.toDouble();
    QString p2 = calculateDegrees(lon);
    QLOG_DEBUG() << "Got longitude " << lonT << ", " << lon << ", degrees: " << p2;
    if (lon >= 0) {
        p2 = p2 + " E";
    } else {
        p2 = p2 + " W";
    }
    const QString &res = p1 + p2;
    QLOG_DEBUG() << "Got location " << res;
    return res;
}

QString LocationDialog::locationText() {
    const QString &latT = latitude.text();
    double lat = latT.toDouble();
    QLOG_DEBUG() << "Got latitude " << latT << " => " << lat;

    const QString &lonT = longitude.text();
    double lon = lonT.toDouble();
    QLOG_DEBUG() << "Got longitude " << lonT << " => " << lon;

    QString res = QString::number(lat) + "," + QString::number(lon);


    const QString &altT = altitude.text();
    double alt = altT.toDouble();
    if (alt > 0.0) {
        res.append("," + QString::number(alt));
    }

    QLOG_DEBUG() << "Got location " << res;
    return res;
}


QString LocationDialog::calculateDegrees(double value) {
    qint32 degrees = floor(value);
    value = (value - degrees) * 60;
    qint32 minutes = floor(value);
    value = value - minutes;
    qint32 seconds = floor(value) * 60;

    QString retval = QString::number(degrees) + "°" +
                     QString::number(minutes) + "'" +
                     QString::number(seconds);
    return retval;
}


double LocationDialog::getLongitude() {
    return longitude.text().toDouble();
}


double LocationDialog::getLatitude() {
    return latitude.text().toDouble();
}


double LocationDialog::getAltitude() {
    return altitude.text().toDouble();
}


void LocationDialog::setAltitude(double value) {
    altitude.setText(QString::number(value));
}


void LocationDialog::setLongitude(double value) {
    longitude.setText(QString::number(value));
}


void LocationDialog::setLatitude(double value) {
    latitude.setText(QString::number(value));
}
