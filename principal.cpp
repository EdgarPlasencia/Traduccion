#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QRectF>
#include <QAction>

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;


    connect(ui->actionLibre, SIGNAL(changed()),this,SLOT(setCheckFormas1()));
    connect(ui->actionLineas, SIGNAL(changed()),this,SLOT(setCheckFormas2()));
    connect(ui->actionRectangulo, SIGNAL(changed()),this,SLOT(setCheckFormas3()));
    connect(ui->actionCircunferencias, SIGNAL(changed()),this,SLOT(setCheckFormas4()));
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::setCheckFormas2()
{
    ui->actionLibre->setChecked(false);
    ui->actionRectangulo->setChecked(false);
    ui->actionCircunferencias->setChecked(false);

}

void Principal::setCheckFormas3()
{
    ui->actionLineas->setChecked(false);
    ui->actionLibre->setChecked(false);
    ui->actionCircunferencias->setChecked(false);
}

void Principal::setCheckFormas4()
{
    ui->actionLineas->setChecked(false);
    ui->actionLibre->setChecked(false);
    ui->actionRectangulo->setChecked(false);
}

void Principal::setCheckFormas1()
{
    ui->actionLineas->setChecked(false);
    ui->actionRectangulo->setChecked(false);
    ui->actionCircunferencias->setChecked(false);
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    mPuedeDibujar = true;

    mInicial = event->pos();
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        event->accept();
        return;
    }


    // Capturar el punto donde se suelta el mouse

    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);


    if(ui->actionLibre->isChecked()){

       mFinal = event->pos();

       mPainter->drawLine(mInicial,mFinal);

       // Mostrar el número de líneas en la barra de estado
       ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
       // Actualizar la interfaz
       update();
       // actualizar el punto inicial
       mInicial = mFinal;

    }

}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{


    mPuedeDibujar = true;
    // Aceptar el vento

    mFinal = event->pos();

    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);




    if(ui->actionRectangulo->isChecked()){

        if(mInicial!=mFinal){
        //LAS DIMENSIONES ES IGUAL A LA DIFERENCIA DE POSICION FINAL E INICIAL

        mPainter->drawRect(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());

        //COMPROBAR PUNTOS
        //qDebug()<<mInicial.x()<<mInicial.y()<<mFinal.x()<<mFinal.y();

        ui->statusbar->showMessage("SE DIBUJO");

        update();

        event->accept();

        }
        }

    if(ui->actionLineas->isChecked()){
       mPainter->drawLine(mInicial.x(),mInicial.y(),mFinal.x(),mFinal.y());
       ui->statusbar->showMessage("SE DIBUJO");

       update();

       event->accept();
    }

    if(ui->actionCircunferencias->isChecked()){
        mPainter->drawEllipse(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());

        ui->statusbar->showMessage("SE DIBUJO");

        update();

        event->accept();
    }

    event->accept();

}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes (*.png)");
    if ( !nombreArchivo.isEmpty() ){
        if (mImagen->save(nombreArchivo))
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}
