/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   16 mai 2006
  Version   :   
  Role      :   Classe IO image pour le format ONERA
  $Id$

=========================================================================*/
#ifndef __otbONERAImageIO_h
#define __otbONERAImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

/* C++ Libraries */
#include <string>
#include "stdlib.h"

/* ITK Libraries */
#include "itkImageIOBase.h"

#include "otbMetaDataKey.h"


namespace otb
{

/** \class ONERAImageIO
 *
 * \brief ImageIO object for reading (not writing) ONERA format images
 *
 * The streaming read is implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT GDALImageIO : public itk::ImageIOBase,
			       public MetaDataKey
{
public:

  typedef unsigned char InputPixelType;
  
  /** Standard class typedefs. */
  typedef ONERAImageIO            Self;
  typedef itk::ImageIOBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ONERAImageIO, itk::ImageIOBase);


  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);
  
  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead(){  return true; };

  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();
 
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);
    
  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite() { return false; };

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);

  void SampleImage(void* buffer,int XBegin, int YBegin, int SizeXRead, int SizeYRead, int XSample, int YSample);
  
protected:
  /** Construtor.*/
  ONERAImageIO();
  /** Destructor.*/
  ~ONERAImageIO();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Read all information on the image*/
  void InternalReadImageInformation();
  /** Dimension along Ox of the image*/
  int m_width;
  /** Dimension along Oy of the image*/
  int m_height;
  /** Number of bands of the image*/
  int m_NbBands;
  /** Buffer*/
  //float **pafimas;
  
  const char* m_currentfile;

private:
  ONERAImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** M�thode analyse le nom du fichier a ouvrir. S'il s'agit d'un r�pertoire, 
    * on regarde s'il contient un produit le fichier ent�te (fichier "ENT...")
    * Dans ce cas, ONERAFileName contient le nom du fichier a ouvrir. 
    * Sinon ONERAFileName contient filename
    */
  void GetOneraImageFileName( const char * filename, std::string & OneraFileName );


  /** Nombre d'octets par pixel */
  int           m_NbOctetPixel;
  

};

} // end namespace otb

#endif // __otbONERAImageIO_h
