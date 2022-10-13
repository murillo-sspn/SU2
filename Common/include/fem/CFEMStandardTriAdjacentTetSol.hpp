/*!
 * \file CFEMStandardTriAdjacentTetSol.hpp
 * \brief Class for the FEM standard surface triangle element
 *        adjacent to a tetrahedron for the solution.
 *        The functions are in the <i>CFEMStandardTriAdjacentTetSol.cpp</i> file.
 * \author E. van der Weide
 * \version 7.1.1 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once 

#include "CFEMStandardTetBase.hpp"
#include "CGemmStandard.hpp"

/*!
 * \class CFEMStandardTriAdjacentTetSol.
 * \brief Class which defines the variables and methods for the triangle 
 *        standard surface element adjacent to a tetrahedron for the solution.
 * \author E. van der Weide
 * \version 7.1.1 "Blackbird"
 */
class CFEMStandardTriAdjacentTetSol final: public CFEMStandardTetBase {

public:

  /*-----------------------------------------------------------------------------------*/
  /*---                     Constructors and destructors.                           ---*/
  /*-----------------------------------------------------------------------------------*/

  /*!
   * \brief Default constructor of the class, deleted to make sure the
   *        overloaded constructor is always used.
   */
  CFEMStandardTriAdjacentTetSol() = delete;

  /*!
   * \overload
   * \param[in] val_nPoly       - Polynomial degree of the grid for this element.
   * \param[in] val_orderExact  - Polynomial degree that must be integrated exactly.
   * \param[in] val_faceID_Elem - This is the face ID of the adjacent volume element
   *                              to which this surface element corresponds.
   * \param[in] val_orientation - Orientation of this surface element relative to
   *                              the adjacent volume element.
   * \param[in] val_gemm_1      - Pointer to the gemm type used to carry out the
   *                              first gemm functionality for this standard face.
   * \param[in] val_gemm_2      - Pointer to the gemm type used to carry out the
   *                              second gemm functionality for this standard face.
   */
  CFEMStandardTriAdjacentTetSol(const unsigned short val_nPoly,
                                const unsigned short val_orderExact,
                                const unsigned short val_faceID_Elem,
                                const unsigned short val_orientation,
                                CGemmBase           *val_gemm_1,
                                CGemmBase           *val_gemm_2);

  /*!
   * \brief Destructor. Nothing to be done.
   */
  ~CFEMStandardTriAdjacentTetSol() = default;

  /*-----------------------------------------------------------------------------------*/
  /*---                  Inline public member functions.                            ---*/
  /*-----------------------------------------------------------------------------------*/

  /*!
   * \brief Function, that returns the padded number of solution DOFs of the volume element.
   * \return The padded number of solution DOFs of the adjacent volume element.
   */
  inline unsigned short GetNSolDOFsPad(void) const override {
   const unsigned short nSol = (nPoly+1)*(nPoly+2)*(nPoly+3)/6;
   return PaddedValue(nSol);
  }

  /*-----------------------------------------------------------------------------------*/
  /*---                     Public member functions.                                ---*/
  /*-----------------------------------------------------------------------------------*/

  /*!
   * \brief Function, that computes the gradient of the solution in integration points.
   * \param[in]  matSolDOF     - Matrix that contains the modal solution DOFs.
   * \param[out] matGradSolInt - Vector of matrices the contains the gradients of the
   *                             solution in the integration points.
   */
  void GradSolIntPoints(ColMajorMatrix<su2double>          &matSolDOF,
                        vector<ColMajorMatrix<su2double> > &matGradSolInt) override;

  /*!
   * \brief Function that computes the solution in integration points.
   * \param[in]  matSolDOF - Matrix that contains the modal solution DOFs.
   * \param[out] matSolInt - Matrix that contains the solution in the integration points.
   */
  void SolIntPoints(ColMajorMatrix<su2double> &matSolDOF,
                    ColMajorMatrix<su2double> &matSolInt) override;

  /*!
   * \brief Function, that updates the residuals of the DOFs with the integral of the
   *        product of the given scalar data and the basis function. The integral is
   *        approximated by the weighted sum of the data in the integration points.
   * \param[in]     scalarDataInt - The scalar data in the integration points that must
   *                                be multiplied by the basis functions.
   * \param[in,out] resDOFs       - The residual of the DOFs that must be updated.
   */
  void ResidualBasisFunctions(ColMajorMatrix<su2double> &scalarDataInt,
                              ColMajorMatrix<su2double> &resDOFs) override;

  /*!
   * \brief Function, that updates the residuals of the DOFs with the integral of the
   *        dot product of the given vector data and the gradient of the basis function.
   *        The integral is approximated by the weighted sum of the data in the integration points.
   * \param[in]     vectorDataInt - The vector data in the integration points that must
   *                                be multiplied by the gradient of the basis functions.
   * \param[in,out] resDOFs       - The residual of the DOFs that must be updated.
   */
  void ResidualGradientBasisFunctions(vector<ColMajorMatrix<su2double> > &vectorDataInt,
                                      ColMajorMatrix<su2double>          &resDOFs) override;

private:

  unsigned short faceID_Elem;   /*!< \brief Face ID of the adjacent tetrahedron, which corresponds to this face. */
  unsigned short orientation;   /*!< \brief Orientation of this face relative to the adjacent tetrahedron. */

  CGemmStandard *gemmDOFs2Int = nullptr; /*!< \brief Pointer to the gemm type used to to compute the data in the
                                                     integration points of the face from the volume DOFs. */
  CGemmStandard *gemmInt2DOFs = nullptr; /*!< \brief Pointer to the gemm type used to to compute the data in the
                                                     volume DOFs from the integration points of the face. */

  ColMajorMatrix<passivedouble> legBasisInt;             /*!< \brief The values of the Legendre basis functions
                                                                     in the integration points of the face. */
  vector<ColMajorMatrix<passivedouble> > derLegBasisInt; /*!< \brief The values of the derivatives of the Legendre
                                                                     basis functions in the integration points. It is a vector,
                                                                     because there are derivatives in three directions. */

  ColMajorMatrix<passivedouble> legBasisIntTranspose;             /*!< \brief Transpose of legBasisInt. */
  vector<ColMajorMatrix<passivedouble> > derLegBasisIntTranspose; /*!< \brief Transpose of derLegBasisInt. */
};
