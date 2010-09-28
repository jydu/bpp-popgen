//
// File GeneMapperCsvExport.h
// Author : Sylvain Gaillard
// Last modification : April 2, 2008
//

/*
   Copyright or © or Copr. CNRS, (April 2, 2008)

   This software is a computer program whose purpose is to provide classes
   for population genetics analysis.

   This software is governed by the CeCILL  license under French law and
   abiding by the rules of distribution of free software.  You can  use, 
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info". 

   As a counterpart to the access to the source code and  rights to copy,
   modify and redistribute granted by the license, users are provided only
   with a limited warranty  and the software's author,  the holder of the
   economic rights,  and the successive licensors  have only  limited
   liability. 

   In this respect, the user's attention is drawn to the risks associated
   with loading,  using,  modifying and/or developing or reproducing the
   software by the user in light of its specific status of free software,
   that may mean  that it is complicated to manipulate,  and  that  also
   therefore means  that it is reserved for developers  and  experienced
   professionals having in-depth computer knowledge. Users are therefore
   encouraged to load and test the software's suitability as regards their
   requirements in conditions enabling the security of their systems and/or 
   data to be ensured and,  more generally, to use and operate it in the 
   same conditions as regards security. 

   The fact that you are presently reading this means that you have had
   knowledge of the CeCILL license and that you accept its terms.
   */

#ifndef _GENEMAPPERCSVEXPORT_H_
#define _GENEMAPPERCSVEXPORT_H_

#include <Bpp/Exceptions.h>
#include <Bpp/Io/FileTools.h>
#include <Bpp/Text/TextTools.h>
#include <Bpp/Text/StringTokenizer.h>
#include <Bpp/Numeric/DataTable.h>

// From local Pop
#include "AbstractIDataSet.h"
#include "BasicAlleleInfo.h"
#include "MultiAlleleMonolocusGenotype.h"

namespace bpp
{

  /**
   * @brief The GeneMapperCsvExport input format for popgenlib.
   *
   * This input format takes a csv file exported from GeneMapper® (Applied Biosystems).
   *
   * @author Sylvain Gaillard
   */
  class GeneMapperCsvExport: public AbstractIDataSet
  {

    private:
      bool IndependentAlleles_;

    public: // Constructor and destructor
      GeneMapperCsvExport(bool ia = false);
      ~GeneMapperCsvExport();

    //public:
      /**
       * @brief Set if allels are concidered as independent markers.
       *
       */
      //SetAllelsAsIndependent(bool flag);

    public:
      /**
       * @name The IDataSet interface.
       * @{
       */
      void read(std::istream& is, DataSet& data_set) throw (Exception);
      void read(const std::string& path, DataSet& data_set) throw (Exception);
      DataSet* read(std::istream& is) throw (Exception);
      DataSet* read(const std::string& path) throw (Exception);
      /**
       * @}
       */

      /**
       * @name The IOFormat interface
       * @{
       */
      virtual const std::string getFormatName() const {
        return "GeneMapper® cvs export";
      }
      virtual const std::string getFormatDescription() const {
        return "GeneMapper® is a flexible genotyping software package that provides DNA sizing and quality allele calls for all Applied Biosystems electrophoresis-based genotyping systems.";
      }
      /**
       * @}
       */
  };

} //end of namespace bpp;

#endif // _GENEMAPPERCSVEXPORT_H_

