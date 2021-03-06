/**
 * @file src/fileinfo/file_presentation/getters/iterative_getter/iterative_distribution_getter/import_table_plain_getter.cpp
 * @brief Methods of ImportTablePlainGetter class.
 * @copyright (c) 2017 Avast Software, licensed under the MIT license
 */

#include "tl-cpputils/conversion.h"
#include "tl-cpputils/string.h"
#include "fileformat/utils/conversions.h"
#include "fileinfo/file_presentation/getters/iterative_getter/iterative_distribution_getter/import_table_plain_getter.h"

using namespace tl_cpputils;
using namespace fileformat;

namespace fileinfo {

namespace
{

const std::size_t distributionArray[] = {6, 40, 20, 11, 11};
const std::string headerArray[] = {"i", "name", "libName", "ordNum", "address"};
const std::string headerDesc[] = {"index", "name of import", "name of library from which is import imported",
									"ordinal number of import", "address of import"};

} // anonymous namespace

/**
 * Constructor
 * @param fileInfo Information about file
 */
ImportTablePlainGetter::ImportTablePlainGetter(FileInformation &fileInfo) : IterativeDistributionGetter(fileInfo)
{
	numberOfStructures = 1;
	numberOfStoredRecords.push_back(fileinfo.getNumberOfStoredImports());
	numberOfExtraElements.push_back(0);
	title = "Import table";
	distribution.insert(distribution.begin(), std::begin(distributionArray), std::end(distributionArray));
	commonHeaderElements.insert(commonHeaderElements.begin(), std::begin(headerArray), std::end(headerArray));
	commonHeaderDesc.insert(commonHeaderDesc.begin(), std::begin(headerDesc), std::end(headerDesc));
	loadRecords();
}

/**
 * Destructor
 */
ImportTablePlainGetter::~ImportTablePlainGetter()
{

}

std::size_t ImportTablePlainGetter::getBasicInfo(std::size_t structIndex, std::vector<std::string> &desc, std::vector<std::string> &info) const
{
	if(structIndex >= numberOfStructures || !fileinfo.hasImportTableRecords())
	{
		return 0;
	}

	desc.clear();
	info.clear();

	desc.push_back("Number of imports: ");
	desc.push_back("CRC32            : ");
	desc.push_back("MD5              : ");
	desc.push_back("SHA256           : ");
	info.push_back(numToStr(fileinfo.getNumberOfStoredImports()));
	info.push_back(fileinfo.getImphashCrc32());
	info.push_back(fileinfo.getImphashMd5());
	info.push_back(fileinfo.getImphashSha256());

	return info.size();
}

bool ImportTablePlainGetter::loadRecord(std::size_t structIndex, std::size_t recIndex, std::vector<std::string> &record)
{
	if(structIndex >= numberOfStructures || recIndex >= numberOfStoredRecords[structIndex])
	{
		return false;
	}

	record.clear();
	record.push_back(numToStr(recIndex));
	record.push_back(replaceNonprintableChars(fileinfo.getImportName(recIndex)));
	record.push_back(replaceNonprintableChars(fileinfo.getImportLibraryName(recIndex)));
	record.push_back(fileinfo.getImportOrdinalNumberStr(recIndex, std::dec));
	record.push_back(fileinfo.getImportAddressStr(recIndex, hexWithPrefix));
	return true;
}

bool ImportTablePlainGetter::getFlagDescriptors(std::size_t structIndex, std::vector<std::string> &desc, std::vector<std::string> &abbv) const
{
	if(structIndex >= numberOfStructures)
	{
		return false;
	}

	desc.clear();
	abbv.clear();

	return true;
}

} // namespace fileinfo
