//
// File PolymorphismMultiGContainerTools.cpp
// Author : Sylvain Gailard
//          Khalid Belkhir
// Last modification : june 15 2006
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#include "PolymorphismMultiGContainerTools.h"

#include <Bpp/Numeric/Random/RandomTools.h>
#include <algorithm>

using namespace std;
using namespace bpp;

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer> PolymorphismMultiGContainerTools::permuteMultiG(
  const PolymorphismMultiGContainer& pmgc)
{
  auto permutedPmgc = make_unique<PolymorphismMultiGContainer>(pmgc);
  vector<size_t> groups;
  for (size_t i = 0; i < permutedPmgc->size(); ++i)
  {
    groups.push_back(permutedPmgc->getGroupId(i));
  }
  std::shuffle(groups.begin(), groups.end(), RandomTools::DEFAULT_GENERATOR);
  for (size_t i = 0; i < permutedPmgc->size(); ++i)
  {
    permutedPmgc->setGroupId(i, groups[i]);
  }
  return permutedPmgc;
}

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer>PolymorphismMultiGContainerTools::permuteMonoG(
    const PolymorphismMultiGContainer& pmgc,
    const std::set<size_t>& groups)
{
  unique_ptr<PolymorphismMultiGContainer> permutedPmgc;
  size_t locNum = pmgc.getNumberOfLoci();
  vector<vector<unique_ptr<const MonolocusGenotypeInterface>>> monoGens;
  monoGens.resize(locNum);
  // Get all the MonolocusGenotypes to permute
  for (size_t i = 0; i < pmgc.size(); ++i)
  {
    if (groups.find(pmgc.getGroupId(i)) != groups.end())
    {
      for (size_t j = 0; j < locNum; ++j)
      {
        monoGens[j].push_back(unique_ptr<const MonolocusGenotypeInterface>(pmgc.multilocusGenotype(i).monolocusGenotype(j).clone()));
      }
    }
  }
  // PermutE the MonolocusGenotypes
  for (size_t i = 0; i < locNum; ++i)
  {
    std::shuffle(monoGens[i].begin(), monoGens[i].end(), RandomTools::DEFAULT_GENERATOR);
  }
  // Build the new PolymorphismMultiGContainer
  size_t k = 0;
  for (size_t i = 0; i < pmgc.size(); ++i)
  {
    if (groups.find(pmgc.getGroupId(i)) != groups.end())
    {
      auto tmpMg = make_unique<MultilocusGenotype>(locNum);
      for (size_t j = 0; j < locNum; ++j)
      {
        if (monoGens[j][k])
          tmpMg->setMonolocusGenotype(j, *(monoGens[j][k]));
      }
      permutedPmgc->addMultilocusGenotype(tmpMg, pmgc.getGroupId(i));
      k++;
    }
    else
    {
      auto tmpMg = make_unique<MultilocusGenotype>(pmgc.multilocusGenotype(i));
      permutedPmgc->addMultilocusGenotype(tmpMg, pmgc.getGroupId(i));
    }
  }

  // update group names
  set<size_t> grpIds = pmgc.getAllGroupsIds();
  for (auto& id : grpIds)
  {
    string name = pmgc.getGroupName(id);
    permutedPmgc->setGroupName(id, name);
  }

  return permutedPmgc;
}

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer> PolymorphismMultiGContainerTools::permuteIntraGroupMonoG(
    const PolymorphismMultiGContainer& pmgc,
    const set<size_t>& groups)
{
  auto permutedPmgc = make_unique<PolymorphismMultiGContainer>();
  size_t locNum = pmgc.getNumberOfLoci();
  vector<vector<unique_ptr<const MonolocusGenotypeInterface>>> monoGens;
  monoGens.resize(locNum);

  for (auto& g : groups) // for each group
  {
    size_t nbIndInGroup = 0;
    // Get all the MonolocusGenotypes of group g to permute
    for (size_t i = 0; i < pmgc.size(); ++i)
    {
      size_t indivGrp = pmgc.getGroupId(i);
      if (groups.find(indivGrp) != groups.end())
      {
        if (indivGrp == g)
        {
          nbIndInGroup++;

          for (size_t j = 0; j < locNum; ++j)
          {
            monoGens[j].push_back(unique_ptr<const MonolocusGenotypeInterface>(pmgc.multilocusGenotype(i).monolocusGenotype(j).clone()));
          }
        }
      }
      else // insert as is
      {
        auto tmpMg = make_unique<MultilocusGenotype>(pmgc.multilocusGenotype(i));
        permutedPmgc->addMultilocusGenotype(tmpMg, indivGrp);
      }
    } // for i

    // Permut the MonolocusGenotypes
    if (nbIndInGroup > 0)
    {
      for (size_t j = 0; j < locNum; ++j)
      {
        std::shuffle(monoGens[j].begin(), monoGens[j].end(), RandomTools::DEFAULT_GENERATOR);
      }

      // Build the new multilocus genotypes
      auto tmpMg = make_unique<MultilocusGenotype>(locNum);
      for (size_t k = 0; k < nbIndInGroup; ++k)
      {
        for (size_t j = 0; j < locNum; ++j)
        {
          if (monoGens[j][k])
            tmpMg->setMonolocusGenotype(j, *(monoGens[j][k]));
        } // for j

        permutedPmgc->addMultilocusGenotype(tmpMg, g);
      } // for k
    } // if nbIndInGroup
  } // for g

  // update groups names
  set<size_t> grpIds = pmgc.getAllGroupsIds();
  for (auto& id : grpIds)
  {
    string name = pmgc.getGroupName(id);
    permutedPmgc->setGroupName(id, name);
  }

  return permutedPmgc;
}

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer> PolymorphismMultiGContainerTools::permuteAlleles(
    const PolymorphismMultiGContainer& pmgc,
    const std::set<size_t>& groups)
{
  auto permutedPmgc = make_unique<PolymorphismMultiGContainer>();
  size_t locNum = pmgc.getNumberOfLoci();
  vector<vector<size_t>> alleles;
  alleles.resize(locNum);
  // Get all the alleles to permut
  for (size_t i = 0; i < pmgc.size(); ++i)
  {
    if (groups.find(pmgc.getGroupId(i)) != groups.end())
    {
      for (size_t j = 0; j < locNum; ++j)
      {
        if (!pmgc.multilocusGenotype(i).isMonolocusGenotypeMissing(j))
          for (size_t k = 0; k < pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex().size(); ++k)
          {
            alleles[j].push_back(pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex()[k]);
          }
      }
    }
  }
  // Permut the alleles
  for (size_t i = 0; i < locNum; ++i)
  {
    std::shuffle(alleles[i].begin(), alleles[i].end(), RandomTools::DEFAULT_GENERATOR);
  }
  // Build the new PolymorphismMultiGContainer
  vector<size_t> k(locNum, 0);
  for (size_t i = 0; i < pmgc.size(); ++i)
  {
    if (groups.find(pmgc.getGroupId(i)) != groups.end())
    {
      auto tmpMg = make_unique<MultilocusGenotype>(locNum);
      for (size_t j = 0; j < locNum; ++j)
      {
        if (!pmgc.multilocusGenotype(i).isMonolocusGenotypeMissing(j))
        {
          if (pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex().size() == 1)
            tmpMg->setMonolocusGenotype(j, MonoAlleleMonolocusGenotype(alleles[j][k[j]++]));
          if (pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex().size() == 2)
            tmpMg->setMonolocusGenotype(j, BiAlleleMonolocusGenotype(alleles[j][k[j]++], alleles[j][k[j]++]));
        }
      }
      permutedPmgc->addMultilocusGenotype(tmpMg, pmgc.getGroupId(i));
    }
    else
    {
      auto tmpMg = make_unique<MultilocusGenotype>(pmgc.multilocusGenotype(i));
      permutedPmgc->addMultilocusGenotype(tmpMg, pmgc.getGroupId(i));
    }
  }

  // update groups names
  set<size_t> grpIds = pmgc.getAllGroupsIds();
  for (auto& id : grpIds) {
    string name = pmgc.getGroupName(id);
    permutedPmgc->setGroupName(id, name);
  }

  return permutedPmgc;
}

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer> PolymorphismMultiGContainerTools::permuteIntraGroupAlleles(
    const PolymorphismMultiGContainer& pmgc,
    const set<size_t>& groups)
{
  auto permutedPmgc = make_unique<PolymorphismMultiGContainer>();
  size_t locNum = pmgc.getNumberOfLoci();
  vector<vector<size_t>> alleles;
  alleles.resize(locNum);

  for (auto& g : groups) // for each group
  {
    size_t nbIndInGroup = 0;

    vector<vector<size_t>> nbAllelesForInds;
    nbAllelesForInds.resize(locNum);
    // Get all the alleles to permute
    for (size_t i = 0; i < pmgc.size(); ++i)
    {
      size_t indivGrp = pmgc.getGroupId(i);
      if (groups.find(indivGrp) != groups.end() )
      {
        if (indivGrp == g)
        {
          nbIndInGroup++;
          for (size_t j = 0; j < locNum; ++j)
          {
            if (!pmgc.multilocusGenotype(i).isMonolocusGenotypeMissing(j))
            {
              size_t nbAlls = pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex().size();
              nbAllelesForInds[j].push_back(nbAlls);
              for (size_t k = 0; k < nbAlls; ++k)
              {
                alleles[j].push_back(pmgc.multilocusGenotype(i).monolocusGenotype(j).getAlleleIndex()[k]);
              }
            }
          }
        }
      }
      else // insert as is
      {
        auto tmpMg = make_unique<MultilocusGenotype>(pmgc.multilocusGenotype(i));
        permutedPmgc->addMultilocusGenotype(tmpMg, indivGrp);
      }
    } // for i

    // Permute the alleles
    if (nbIndInGroup > 0)
    {
      for (size_t i = 0; i < locNum; ++i)
      {
        // alleles[i] = RandomTools::getSample(alleles[i], alleles[i].size());
        std::shuffle(alleles[i].begin(), alleles[i].end(), RandomTools::DEFAULT_GENERATOR);
      }

      // Build the new PolymorphismMultiGContainer
      vector<size_t> k(locNum, 0);

      for (size_t ind = 0; ind < nbIndInGroup; ind++)
      {
        auto tmpMg = make_unique<MultilocusGenotype>(locNum);
        for (size_t j = 0; j < locNum; ++j)
        {
          if (nbAllelesForInds[j][ind] == 1)
            tmpMg->setMonolocusGenotype(j, MonoAlleleMonolocusGenotype(alleles[j][k[j]++]));
          if (nbAllelesForInds[j][ind] == 2)
            tmpMg->setMonolocusGenotype(j, BiAlleleMonolocusGenotype(alleles[j][k[j]++], alleles[j][k[j]++]));
        } // for j

        permutedPmgc->addMultilocusGenotype(tmpMg, g);
      } // for ind
    } // if nbIndInGroup
  } // for g


  // update groups names
  auto grpIds = pmgc.getAllGroupsIds();
  for (auto& id : grpIds)
  {
    string name = pmgc.getGroupName(id);
    permutedPmgc->setGroupName(id, name);
  }

  return permutedPmgc;
}

/******************************************************************************/

unique_ptr<PolymorphismMultiGContainer> PolymorphismMultiGContainerTools::extractGroups(
    const PolymorphismMultiGContainer& pmgc,
    const set<size_t>& groups)
{
  auto subPmgc = make_unique<PolymorphismMultiGContainer>();
  for (auto& g : groups) // for each group
  {
    // Get all the MonolocusGenotypes of group g to extract
    for (size_t i = 0; i < pmgc.size(); ++i)
    {
      size_t indivGrp = pmgc.getGroupId(i);
      if (groups.find(indivGrp) != groups.end() )
      {
        if (indivGrp == g)
        {
	  auto tmpMg = make_unique<MultilocusGenotype>(pmgc.multilocusGenotype(i));
          subPmgc->addMultilocusGenotype(tmpMg, indivGrp);
        }
      }
    } // for i
  } // for g

  // update group names
  auto grpIds = subPmgc->getAllGroupsIds();
  for (auto& id : grpIds)
  {
    string name = pmgc.getGroupName(id);
    subPmgc->setGroupName(id, name);
  }

  return subPmgc;
}

/******************************************************************************/
