//
// File SequenceStatistics.h
// Author : Eric Bazin
//          Sylvain Gaillard
//          khalid Belkhir
// Last modification : Friday December 12 2005
//

/*
   Copyright or © or Copr. CNRS, (November 17, 2004)

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

//Secured inclusion of header's file
#ifndef _SEQUENCESTATISTICS_H_
#define _SEQUENCESTATISTICS_H_

//From the SeqLib library
#include <Seq/SiteIterator.h>
#include <Seq/SiteContainer.h>
#include <Seq/SymbolListTools.h>
#include <Seq/CodonAlphabet.h>
#include <Seq/GeneticCode.h>
#include <Seq/SiteContainerTools.h>

//From the PolyLib library
#include "PolymorphismSequenceContainer.h"

// From the STL
#include <string>
#include <map>
#include <vector>

namespace bpp
{
  /**
   * @brief Static class providing methods to compute statistics on sequences
   * data.
   *
   * @author Sylvain Gaillard
   */
  class SequenceStatistics
  {
    public:
      // Class destructor:
      ~SequenceStatistics();

      /************************************************************************/
    public:

      /**
       * @brief Compute the number of polymorphic site in an alignment
       *
       * The number of polymorphic site is also known as the number of segregating
       * site @f$S@f$.
       *
       * Gaps are consider as mutations so if you want number of
       * polymorphic site without gap, set the gapflag parameter to true.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to
       * take gap into account
       */
      static unsigned int polymorphicSiteNumber(const PolymorphismSequenceContainer & psc, bool gapflag = true);



      /**
       * @brief Compute the number of parsimony informative sites in an alignment
       *
       * @param psc a PolymorphicSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to
       * take gap into account
       */
      static unsigned int parsimonyInformativeSiteNumber(const PolymorphismSequenceContainer & psc, bool gapflag = true);


      /**
       * @brief Count the number of singleton nucleotides in an alignment.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to
       * take gap into account
       */
      static unsigned int countSingleton(const PolymorphismSequenceContainer & psc, bool gapflag = true);

      /**
       * @brief Count the total number of mutations in an alignment.
       *
       * This count is assumed to be under an infinite site model.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to
       * take gap into account
       */
      static unsigned int totNumberMutations(const PolymorphismSequenceContainer & psc, bool gapflag = true);

      /**
       * @brief Count the total number of mutations in external branchs.
       *
       * This is counted as the number of distinct singleton nucleotide  in the ingroup
       * that are not shared with the outgroup
       * A site is ignored if it contains more than one variant in the outgroup
       * A site is ignored if it contains unresolved variants or gaps
       *
       * @param  ing a PolymorphismSequenceContainer the ingroup alignement
       * @param  outg a PolymorphismSequenceContainer the outgroup alignement
       */
      static unsigned int totMutationsExternalBranchs(const PolymorphismSequenceContainer & ing,
          const PolymorphismSequenceContainer outg);


      /**
       * @brief Compute the number of triplet in an alignment
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to take gap into account
       */
      static unsigned int tripletNumber(const PolymorphismSequenceContainer & psc, bool gapflag = true);



      /**
       * @brief Compute the sum of per site heterozygosity in an alignment
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to take gap into account
       */
      static double heterozygosity(const PolymorphismSequenceContainer & psc, bool gapflag=true);


      /**
       * @brief Compute the sum of per site squared heterozygosity in an alignment
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to take gap into account
       */
      static double squaredHeterozygosity(const PolymorphismSequenceContainer & psc, bool gapflag=true);


      /**
       * @brief Compute the mean GC content in an alignment
       *
       * @param psc a PolymorphismSequenceContainer
       */
      static double gcContent(const PolymorphismSequenceContainer & psc);


      /**
       * @brief Return the number of GC alleles and the total number of alleles at polymorphic sites only
       *
       * G vs C and A vs T polymorphism are not taken into account
       *
       * @param psc a PolymorphismSequenceContainer
       * @param stopflag a boolean set by default to true if you don't want
       * to take stop codons into account
       */
      static std::vector<unsigned int> gcPolymorphism(const PolymorphismSequenceContainer & psc, bool stopflag = true);



      /**
       * @brief Compute diversity estimator Theta of Watterson (1975, Theor Popul Biol, 7 pp256-276)
       *
       * @f[
       * \hat{\theta}_S=\frac{S}{a_1}
       * @f]
       * where @f$S@f$ is the number of polymorphic sites and @f$a_1@f$ is describe in SequenceStatistics::_getUsefullValues().
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gap into account
       */
      static double watterson75( const PolymorphismSequenceContainer & psc, bool gapflag = true );


      /**
       * @brief Compute diversity estimator Theta of Tajima (1983, Genetics, 105 pp437-460)
       *
       * @f[
       * \hat{\theta}_\pi=1-\sum_{i=1}^{S}\sum_{j=1}^{4}\frac{k_{j,i}\times\left(k_{j,i}-1\right)}
       * {n_i\times\left(n_i-1\right)} \qquad \textrm{with }k_{j,i}>0
       * @f]
       * where @f$k_{j,i}@f$ is the count of the j<sup>th</sup> state at the i<sup>th</sup> site,
       * @f$n_i@f$ the number of nucleotides and @f$S@f$ the number of polymorphic sites.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gap into account
       */
      static double tajima83( const PolymorphismSequenceContainer & psc, bool gapflag = true );


      /**
       * @brief Return the number of haplotype in the sample.
       * Depaulis and Veuille (1998, Mol Biol Evol, 12 pp1788-1790)
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gap into account
       */
      static unsigned int DVK ( const PolymorphismSequenceContainer & psc, bool gapflag = true );


      /**
       * @brief Return the haplotype diversity of a sample.
       * Depaulis and Veuille (1998, Mol Biol Evol, 12 pp1788-1790)
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gaps into account
       */
      static double DVH ( const PolymorphismSequenceContainer & psc, bool gapflag = true );


      /**
       * @brief Return the number of transitions.
       *
       * @param psc a PolymorphismSequenceContainer
       */
      static unsigned int getNumberOfTransitions( const PolymorphismSequenceContainer & psc );


      /**
       * @brief Return the number of transversions.
       *
       * @param psc a PolymorphismSequenceContainer
       */
      static unsigned int getNumberOfTransversions( const PolymorphismSequenceContainer & psc );


      /**
       * @brief Return the ratio of transitions/transversions.
       *
       * @param psc a PolymorphismSequenceContainer
       */
      static double getTransitionsTransversionsRatio( const PolymorphismSequenceContainer & psc ) throw(Exception);




      /**
       * @brief Compute the number of codon sites with stop codon
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag a boolean set by default to true if you don't want to take gaps into account
       */
      static unsigned int stopCodonSiteNumber(const PolymorphismSequenceContainer & psc, bool gapflag = true);




      /**
       * @brief Compute the number of polymorphic codon with only one mutated site
       *
       * @param psc a PolymorphismSequenceContainer
       * @param stopflag a boolean set by default to true if you don't want
       * to take stop codon neither undefined sites into account
       * @param gapflag a boolean set by default to true if you don't want
       * to take gaps into account
       */
      static unsigned int monoSitePolymorphicCodonNumber(const PolymorphismSequenceContainer & psc, bool stopflag = true, bool gapflag = true);



      /**
       * @brief Compute the number of synonymous polymorphic codon sites
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       */
      static unsigned int synonymousPolymorphicCodonNumber(const PolymorphismSequenceContainer & psc, const GeneticCode & gc);


      /**
       * @brief Compute the Watterson(1975,Theor Popul Biol, 7 pp256-276) estimator for synonymous positions
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * In case of complex codon, the path that gives the minimum number of non-synonymous changes
       * is chosen. The argument minchange=true is sent to numberOfSynonymousDifferences used in this method.
       * Otherwise, a non-integer number could be return.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       */
      static double watterson75Synonymous(const PolymorphismSequenceContainer & psc, const GeneticCode & gc);

      /**
       * @brief Compute the Watterson(1975, Theor Popul Biol, 7 pp256-276) estimator for non synonymous positions
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * In case of complex codon, the path that gives the minimum number of non-synonymous changes
       * is chosen. The argument minchange=true is sent to numberOfSynonymousDifferences used in this method.
       * Otherwise, a non-integer number could be return.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       */
      static double watterson75NonSynonymous(const PolymorphismSequenceContainer & psc, const GeneticCode & gc);

      /**
       * @brief Compute the synonymous nucleotide diversity, pi
       *
       * Gaps and unresolved sites are automatically excluded
       * If minchange = false (default option) the different paths are equally weighted.
       * If minchange = true the path with the minimum number of non-synonymous change is chosen.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param minchange a boolean set to false
       */
      static double piSynonymous(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, bool minchange=false);


      /**
       * @brief Compute the non-synonymous nucleotide diversity, pi
       *
       * Gaps and unresolved sites are automatically excluded
       * If minchange = false (default option) the different paths are equally weighted.
       * If minchange = true the path with the minimum number of non-synonymous change is chosen.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param minchange a boolean set by default to false
       */
      static double piNonSynonymous(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, bool minchange=false);


      /**
       * @brief compute the mean number of synonymous site in an alignment
       *
       * A site is x% synonymous if x% of possible mutations are synonymous
       * The transition/transversion can be taken into account (use the variable ratio)
       * Gaps and unresolved sites are automatically excluded
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param ratio a double
       */
      static double meanSynonymousSitesNumber(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, double ratio=1.0);


      /**
       * @brief compute the mean number of non-synonymous site in an alignment
       *
       * A site is x% synonymous if x% of possible mutations are synonymous
       * The transition/transversion can be taken into account (use the variable ratio)
       * Gaps are automatically excluded
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param ratio a double
       */
      static double meanNonSynonymousSitesNumber(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, double ratio=1.0);


      /**
       * @brief compute the number of synonymous subsitutions in an alignment
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * In case of complex codon, the path that gives the minimum number of non-synonymous changes
       * is chosen. The argument minchange=true is sent to numberOfSynonymousDifferences used in this method.
       * Otherwise, a non-integer number could be return.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
       */
      static unsigned int synonymousSubstitutionsNumber(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, double freqmin = 0);


      /**
       * @brief compute the number of non synonymous subsitutions in an alignment
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * In case of complex codon, the path that gives the minimum number of non-synonymous changes
       * is chosen. The argument minchange=true is sent to numberOfSynonymousDifferences used in this method.
       * Otherwise, a non-integer number could be return.
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
       */
      static unsigned int nonSynonymousSubstitutionsNumber(const PolymorphismSequenceContainer & psc, const GeneticCode & gc, double freqmin = 0);


      /**
       * @brief compute the number of fixed differences between two alignements
       *
       * Gaps and unresolved sites are automatically excluded
       *
       * In case of complex codon, the path that gives the minimum number of non-synonymous changes
       * is chosen. The argument minchange=true is sent to numberOfSynonymousDifferences used in this method.
       * Otherwise, a non-integer number could be return.
       * @param pscin a PolymorphismSequenceContainer
       * @param pscout a PolymorphismSequenceContainer
       * @param psccons a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       */
      static std::vector<unsigned int> fixedDifferences(const PolymorphismSequenceContainer & pscin, const PolymorphismSequenceContainer & pscout, PolymorphismSequenceContainer & psccons, const GeneticCode & gc);

      /**
       * @brief return a vector containing Pa, Ps, Da, Ds
       *
       * Gaps and unresolved sites are automatically excluded
       * @param ingroup a PolymorphismSequenceContainer
       * @param outgroup a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
       */
      static std::vector<unsigned int> MKtable(const PolymorphismSequenceContainer & ingroup, const PolymorphismSequenceContainer & outgroup , const GeneticCode & gc, double freqmin = 0);


      /**
       * @brief return the neutrality index NI = (Pa/Ps)/(Da/Ds) (Rand & Kann 1996, Mol. Biol. Evol. 13 pp735-748)
       *
       * Return -1 if Ps or Da are zero
       * Gaps and unresolved sites are automatically excluded
       *
       * @param ingroup a PolymorphismSequenceContainer
       * @param outgroup a PolymorphismSequenceContainer
       * @param gc a GeneticCode
       * @param freqmin a double, to exclude snp in frequency strictly lower than freqmin
       */
      static double neutralityIndex(const PolymorphismSequenceContainer & ingroup, const PolymorphismSequenceContainer & outgroup , const GeneticCode & gc, double freqmin = 0);



      /**
       * @brief Return the Tajima's D test (Tajima 1989, Genetics 123 pp 585-595).
       *
       * Calculation using the number of polymorphic (segregating) sites.
       * @f[
       * D=\frac{\hat{\theta}_\pi-\hat{\theta}_S}{\sqrt{\textrm{V}\left(\hat{\theta}_\pi-\hat{\theta}_S\right)}}
       * =\frac{\hat{\theta}_\pi-\hat{\theta}_S}{\sqrt{e_1S+e_2S(S-1)}}
       * @f]
       *
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gap into account
       */
      static double tajimaDSS(const PolymorphismSequenceContainer & psc, bool gapflag = true);


      /**
       * @brief Return the Tajima's D test (Tajima 1989, Genetics 123 pp 585-595).
       *
       * Calculation using the total number of mutation.
       * @f[
       * D=\frac{\hat{\theta}_\pi-\frac{\eta}{a_1}}{\sqrt{e_1\eta+e_2\eta(\eta-1)}}
       * @f]
       * @param psc a PolymorphismSequenceContainer
       * @param gapflag flag set by default to true if you don't want to
       * take gap into account
       */
      static double tajimaDTNM(const PolymorphismSequenceContainer & psc, bool gapflag = true);


      /**
       * @brief Return the Fu and Li D test  (Fu & Li 1993, Genetics, 133 pp693-709).
       *
       * @param ingroup a PolymorphismSequenceContainer
       * @param outgroup a PolymorphismSequenceContainer
       *
       * This version uses the number of mutations
       * If the outgroup contains more than one sequence the sites with more than one
       * variant will not be considered for external branch mutations !
       */
      static double fuliD(const PolymorphismSequenceContainer & ingroup, const PolymorphismSequenceContainer & outgroup);


      /**
       * @brief Return the Fu and Li D<sup>*</sup> test (Fu & Li 1993, Genetics, 133 pp693-709).
       *
       * @param group a PolymorphismSequenceContainer
       */
      static double fuliDstar(const PolymorphismSequenceContainer & group);



      /**

       * @brief Return the Fu and Li F test (Fu & Li 1993, Genetics, 133 pp693-709).
       *
       * This version uses the number of mutations
       * This version uses the number of mutations
       * If the outgroup contains more than one sequence the sites with more than one
       * variant will not be considered for externalbranch mutations !
       *
       * @param ingroup a PolymorphismSequenceContainer
       * @param outgroup a PolymorphismSequenceContainer
       */
      static double fuliF(const PolymorphismSequenceContainer & ingroup, const PolymorphismSequenceContainer & outgroup);


      /**
       * @brief Return the Fu and Li F<sup>*</sup> test (Fu & Li 1993, Genetics, 133 pp693-709).
       *
       * @param group a PolymorphismSequenceContainer
       */
      static double fuliFstar(const PolymorphismSequenceContainer & group);


      /**
       * @brief generate a special PolymorphismSequenceContainer for linkage disequilbrium analysis
       *
       * Create a PolymorphismSequenceContainer with only polymorphic site :
       * The value 1 is assigned to the most frequent allele, and 0 to the least frequent
       * This psc is needed to compute Linkage Disequilibrium Statistics
       * Should be used before excluding gaps, but sites with gaps are not counted as polymorphic sites
       * Singleton can be excluded
       * Polymorphix site with the lowest frequency < threshold can be excluded
       * Only polymorphic sites with 2 alleles are kept
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       */
      static PolymorphismSequenceContainer * generateLDContainer(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0);


      /**
       * @brief give the vector of the pairwise distances between site positions corresponding to a LD SequencePolymorphismContainer
       *
       * Assume that all sequences have the same length
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites is lower than 2
       */
      static Vdouble pairwiseDistances1(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);



      /**
       * @brief give the vector of all mean pairwise distance between two sites to a LD SequencePolymorphismContainer
       *
       * pairwise distances are computed for each sequence separately, excluding gaps. Then the mean is taken over all the sequences
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites is lower than 2
       */
      static Vdouble pairwiseDistances2(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the vector of all mean pairwise D value between two sites (Lewontin & Kojima 1964, Evolution 14 pp458-472)
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble pairwiseD(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the vector of all mean pairwise D' value between two sites (Lewontin 1964, Genetics 49 pp49-67))
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble pairwiseDprime(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the vector of all mean pairwise R² value between two sites (Hill & Robertson 1968, Theor. Appl. Genet., 38 pp226-231)
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble pairwiseR2(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give mean D over all pairwise comparisons
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double meanD(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give mean D' over all pairwise comparisons
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double meanDprime(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give mean R² over all pairwise comparisons
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double meanR2(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give mean pairwise distances between sites / method 1: differences between sequences are not taken into account
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites is lower than 2
       */
      static double meanDistance1(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);



      /**
       * @brief give mean pairwise distances between sites / method 2: differences between sequences are taken into account
       *
       * @param psc a PolymorphismSequenceContainer
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites is lower than 2
       */
      static double meanDistance2(const PolymorphismSequenceContainer & psc, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope of the regression |D| = 1+a*distance
       *
       * The slope is given in |D| per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double originRegressionD(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope of the regression |D'| = 1+a*distance
       *
       * The slope is given in |D'| per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double originRegressionDprime(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope of the regression R² = 1+a*distance
       *
       * The slope is given in R² per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double originRegressionR2(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope and the origin of the regression |D| = a*distance+b
       *
       * The slope is given in |D| per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble linearRegressionD(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope and the origin of the regression |D'| = a*distance+b
       *
       * The slope is given in |D'| per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble linearRegressionDprime(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope and the origin of the regression R² = a*distance+b
       *
       * The slope is given in R² per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static Vdouble linearRegressionR2(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give the slope of the regression R² = 1/(1+a*distance)
       *
       * To fit the theoretical prediction R² = 1/(1+4Nr)
       * The slope is given in R² per kb
       *
       * @param psc a PolymorphismSequenceContainer
       * @param distance1 a boolean (true to use distance1, false to use distance2, false by default)
       * @param keepsingleton a boolean (true by default, false to exclude singleton)
       * @param freqmin a float (to exlude site with the lowest allele frequency less than the threshold given by freqmin, 0 by default)
       * @throw DimensionException if the number of sites or the number of sequences is lower than 2
       */
      static double inverseRegressionR2(const PolymorphismSequenceContainer & psc, bool distance1=false, bool keepsingleton=true, double freqmin=0) throw (DimensionException);


      /**
       * @brief give estimate of C=4Nr using Hudson method (Hudson 1987, Genet. Res., 50 pp245-250)
       *
       * @param psc a PolymorphismSequenceContainer
       * @param precision default value = 0.000001
       * @param cinf initial value, by default cinf=0.001
       * @param csup initial value, by default csup = 10000
       */
      static double hudson87(const PolymorphismSequenceContainer & psc, double precision = 0.000001, double cinf=0.001, double csup=10000);

      /**
       * @brief Test usefull values
       * @param s a ostream where write the values
       * @param n then number of observed sequences
       * @author Sylvain Gaillard
       */
      static void testUsefullvalues(ostream & s, unsigned int n);


    private:
      /**
       * @brief Count the number of mutation for a site.
       */
      static unsigned int getMutationNumber_(const Site & site);

      /**
       * @brief Count the number of singleton for a site.
       */
      static unsigned int getSingletonNumber_(const Site & site);

      /**
       * @brief Count the number of singleton for a site.
       *
       * will count singletons that are not in site_out (a site in outgroup)
       * site_in is a site from an ingroup
       */
      static unsigned int getDerivedSingletonNumber_(const Site & site_in,const Site & site_out );


      /**
       * @brief Get usefull values for theta estimators.
       *
       * @param n the number of observed sequences
       *
       * @return A map with 11 values. Keys are a1, a2, a1n, b1, b2, c1, c2, cn, dn, e1 and e2.
       * The values are :
       * @f[
       * a_1=\sum_{i=1}^{n-1}\frac{1}{i} \qquad a_2=\sum_{i=1}^{n-1}\frac{1}{i^2}
       * @f]
       * @f[
       * a_{1n}=\sum_{i=1}^{n}\frac{1}{i}
       * @f]
       * @f[
       * b_1=\frac{n+1}{3(n-1)} \qquad b_2=\frac{2(n^2+n+3)}{9n(n-1)}
       * @f]
       * @f[
       * c_1=b_1-\frac{1}{a_1} \qquad c_2=b_2-\frac{n+2}{a_1n}+\frac{a_2}{a_1^2}
       * @f]
       * @f[
       * c_n=2\frac{na_1-2(n-1)}{(n-1)(n-2)}
       * @f]
       * @f[
       * d_n=c_n+\frac{n-2}{(n-1)^2}+\frac{2}{n-1}\left(\frac{3}{2}-\frac{2a_{1n}-3}{n-2}-\frac{1}{n}\right)
       * @f]
       * @f[
       * e_1=\frac{c_1}{a_1} \qquad e_2=\frac{c_2}{a_1^2+a_2}
       * @f]
       * where @f$n@f$ is the number of observed sequences.
       *
       * @author Sylvain Gaillard
       */
      static std::map<std::string, double> getUsefullValues_(unsigned int n);

      /**
       * @brief Get the vD value of equation (32) in Fu & Li 1993, Genetics, 133 pp693-709)
       *
       * @param n the number of observed sequences
       * @param a1 as describe in getUsefullValues
       * @param a2 as describe in getUsefullValues
       * @param cn as describe in getUsefullValues
       *
       * @return the vD value as double
       *
       * @author Sylvain Gaillard
       */
      static double getVD_(unsigned int n, double a1, double a2, double cn);

      /**
       * @brief Get the uD value of equation (32) in Fu & Li 1993, Genetics, 133 pp693-709)
       *
       * @param a1 as describe in getUsefullValues
       * @param vD as provided by getVD_
       *
       * @return the uD value as double
       *
       * @author Sylvain Gaillard
       */
      static double getUD_(double a1, double vD);

      /**
       * @brief Get the vD* value of D* equation in Fu & Li 1993, Genetics, 133 pp693-709)
       *
       * @param n the number of observed sequences
       * @param a1 as describe in getUsefullValues
       * @param a2 as describe in getUsefullValues
       * @param dn as describe in getUsefullValues
       *
       * @return the vD* value as double
       *
       * @author Sylvain Gaillard
       */
      static double getVDstar_(unsigned int n, double a1, double a2, double dn);

      /**
       * @brief Get the uD* value of D* equation in Fu & Li 1993, Genetics, 133 pp693-709)
       *
       * @param n the number of observed sequences
       * @param a1 as describe in getUsefullValues
       * @param vDs as provided by getVDstar_
       *
       * @return the uD* value as double
       *
       * @author Sylvain Gaillard
       */
      static double getUDstar_(unsigned int n, double a1, double vDs);

      /**
       * @brief give the left hand term of equation (4) in Hudson (Hudson 1987, Genet. Res., 50 pp245-250)
       * This term is used in hudson87
       * @param psc a PolymorphismSequenceContainer
       */
      static double leftHandHudson_(const PolymorphismSequenceContainer & psc);

      /**
       * @brief give the right hand term of equation (4) in Hudson (Hudson 1987, Genet. Res., 50 pp245-250)
       * This term is used in hudson87
       */
      static double rightHandHudson_(double c, unsigned int n);

      /************************************************************************/

  };

} //end of namespace bpp;

#endif // _SEQUENCESTATISTICS_H_

