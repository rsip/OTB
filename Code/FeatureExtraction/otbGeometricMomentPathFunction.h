/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   22 mars 2006
  Version   :   
  Role      :   Geometric Moments Class of path 
  $Id$

=========================================================================*/
#ifndef _otbGeometricMomentPathFunction_h
#define _otbGeometricMomentPathFunction_h

#include "otbPathFunction.h"


namespace otb
{

/**
 * \class GeometricMomentPathFunction
 * \brief Virtual class for the Geometric moments for an path function
 *
 * \ingroup PathFunctions
 */

template < class TInputPath,    
           class TOutput >
class ITK_EXPORT GeometricMomentPathFunction :
  public PathFunction< TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef GeometricMomentPathFunction                Self;
  typedef PathFunction< TInputPath, TOutput>         Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricMomentPathFunction, PathFunction);


  /** InputPathType typedef support. */
  typedef typename Superclass::InputPathType         PathType;
  typedef typename Superclass::InputPathConstPointer PathConstPointer;
 
  /** OutputPathType typedef support. */
  typedef typename Superclass::OutputType            OutputType;


protected:
  GeometricMomentPathFunction() {};
  ~GeometricMomentPathFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
     }
     
private:
  GeometricMomentPathFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented
};

} // namespace otb

#endif

