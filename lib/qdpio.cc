// $Id: qdpio.cc,v 1.1 2003-04-15 21:08:23 edwards Exp $
//
/*! @file
 * @brief IO support via QIO
 */

#include "qdp.h"

QDP_BEGIN_NAMESPACE(QDP);

//--------------------------------------------------------------------------------
//! XML reader class
XMLMetaReader::XMLMetaReader() {}
XMLMetaReader::~XMLMetaReader() {}

//! XML writer class
XMLMetaWriter::XMLMetaWriter() {}
XMLMetaWriter::~XMLMetaWriter() {}




//-----------------------------------------
static int get_node_number(const int coord[])
{
  multi1d<int> crd(Nd);
  crd = coord;   // an array copy
  int node = Layout::nodeNumber(crd);
  return node;
}


//-----------------------------------------
// QDP QIO support
QDPSerialReader::QDPSerialReader() {iop=false;}

QDPSerialReader::QDPSerialReader(XMLMetaReader& xml, const char* p) {open(xml,p);}

void QDPSerialReader::open(XMLMetaReader& xml, const char* p) 
{
  QIO_Layout *layout = new QIO_Layout;
  int latsize[Nd];

  for(int m=0; m < Nd; ++m)
    latsize[m] = Layout::lattSize()[m];

  layout->node_number = &get_node_number;
  layout->latsize = latsize; 
  layout->latdim = Nd; 
  layout->volume = Layout::vol(); 
  layout->this_node = Layout::nodeNumber(); 

  if ((qio_in = QIO_open_read(xml.get(), p, QIO_SERIAL, layout)) == NULL)
    QDP_error_exit("QDPSerial::Reader: failed to open file %s",p);

  delete layout;

  iop=true;
}

void QDPSerialReader::close()
{
  if (is_open()) 
  {
    int status = QIO_close_read(qio_in);
    delete qio_in;
  }

  iop = false;
}

bool QDPSerialReader::is_open() {return iop;}

QDPSerialReader::~QDPSerialReader() {close();}


//-----------------------------------------
//! text writer support
QDPSerialWriter::QDPSerialWriter() {iop=false;}

QDPSerialWriter::QDPSerialWriter(const XMLMetaWriter& xml, const char* p) {open(xml,p);}

void QDPSerialWriter::open(const XMLMetaWriter& xml, const char* p) 
{
  QIO_Layout *layout = new QIO_Layout;
  int latsize[Nd];

  for(int m=0; m < Nd; ++m)
    latsize[m] = Layout::lattSize()[m];

  layout->node_number = &get_node_number;
  layout->latsize = latsize; 
  layout->latdim = Nd; 
  layout->volume = Layout::vol(); 
  layout->this_node = Layout::nodeNumber(); 

  if ((qio_out = QIO_open_write(xml.get(), p, QIO_SERIAL, QIO_LEX_ORDER, QIO_CREATE, 
				layout)) == NULL)
    QDP_error_exit("QDPSerial::Writer: failed to open file %s",p);

  delete layout;

  iop=true;
}

void QDPSerialWriter::close()
{
  if (is_open()) 
  {
    int status = QIO_close_write(qio_out);
    delete qio_out;
  }

  iop = false;
}

bool QDPSerialWriter::is_open() {return iop;}

QDPSerialWriter::~QDPSerialWriter() {close();}



QDP_END_NAMESPACE();
