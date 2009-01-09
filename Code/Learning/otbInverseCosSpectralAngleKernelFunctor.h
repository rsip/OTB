/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbInverseCosSpectralAngleKernelFunctor_h
#define __otbInverseCosSpectralAngleKernelFunctor_h

#include "svm.h"

namespace otb
{
  /** \class InverseCosSpectralAngleKernelFunctor
   * \brief Inverse Cosinus Spectral Angle Kernel.
   * Performs an inverse multiquadric kernel evaluation that suit
   * spectral data.
   *
   * It is of kind:
   * \f$ \frac{1}{\sqrt{ c_0 - \cos\textrm{SAM(x,y)} }} \f$ where \f$ \textrm{SAM(x,y)} \f$ is
   * Spectral Angle Measure:
   * \f$ \textrm{SAM(x,y) = \acos\left( \frac{x \cdot y}{\|x\| \|y\|} \right) \f$.
   *
   * The only parameter \f$ c_0 \f$ is to be fixed by the \code SetValue \endcode
   * interface with keyword Coef (def 1.0).
   */

class InverseCosSpectralAngleKernelFunctor
		: public GenericKernelFunctorBase
{
public:

	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;

	InverseCosSpectralAngleKernelFunctor ();
	virtual ~InverseCosSpectralAngleKernelFunctor () { }

	/** Specific implementation of \code Update \endcode to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();

protected:
	inline	double	SAM ( const svm_node * x, const svm_node * y ) const;

	double m_Coef;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseCosSpectralAngleKernelFunctor.txx"
#endif


#endif

