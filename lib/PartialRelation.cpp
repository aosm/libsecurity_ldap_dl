/*
 * Copyright (c) 2004 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#include "PartialRelation.h"
#include "TableRelation.h"
#include "CommonCode.h"



PartialRelation::PartialRelation (CSSM_DB_RECORDTYPE recordType, int numberOfColumns)  :
	Relation (recordType), mNumberOfColumns (numberOfColumns), mColumnNames (NULL),
	mColumnFormat (NULL)
{
}



PartialRelation::~PartialRelation ()
{
	if (mColumnNames != NULL)
	{
		int i;
		for (i = 0; i < mNumberOfColumns; ++i)
		{
			delete mColumnNames[i];
		}
		
		delete mColumnNames;
		delete mColumnFormat;
	}
}



void PartialRelation::SetColumnNames (char* column0, ...)
{
	// make an array of char* big enough to hold our data
	mColumnNames = new Value*[mNumberOfColumns];
	mColumnNames[0] = new StringValue (column0);
	
	va_list argList;
	va_start (argList, column0);

	int i;
	for (i = 1; i < mNumberOfColumns; ++i)
	{
		char* next = va_arg (argList, char*);
		mColumnNames[i] = new StringValue (next);
	}
	
	va_end (argList);
}



void PartialRelation::SetColumnFormats (CSSM_DB_ATTRIBUTE_FORMAT column0, ...)
{
	mColumnFormat = new CSSM_DB_ATTRIBUTE_FORMAT[mNumberOfColumns];
	mColumnFormat[0] = column0;
	
	va_list argList;
	va_start (argList, column0);

	int i;
	for (i = 1; i < mNumberOfColumns; ++i)
	{
		CSSM_DB_ATTRIBUTE_FORMAT next = va_arg (argList, CSSM_DB_ATTRIBUTE_FORMAT);
		mColumnFormat[i] = next;
	}
	
	va_end (argList);
}



void PartialRelation::SetColumnIDs (uint32 column0, ...)
{
	mColumnIDs = new uint32 [mNumberOfColumns];
	mColumnIDs[0] = column0;
	
	va_list argList;
	va_start (argList, column0);

	int i;
	for (i = 1; i < mNumberOfColumns; ++i)
	{
		uint32 next = va_arg (argList, uint32);
		mColumnIDs[i] = next;
	}
	
	va_end (argList);
}



Tuple* PartialRelation::GetColumnNames ()
{
	TableTuple *t = new TableTuple (mColumnNames, mNumberOfColumns);
	return t;
}



int PartialRelation::GetNumberOfColumns ()
{
	return mNumberOfColumns;
}



uint32* PartialRelation::GetColumnIDs ()
{
	return mColumnIDs;
}



int PartialRelation::GetColumnNumber (const char* columnName)
{
	// look for a column name that matches this columnName.  If not, throw an exception
	int i;
	for (i = 0; i < mNumberOfColumns; ++i)
	{
		StringValue* s = (StringValue*) mColumnNames[i];
		if (s->GetRawValueAsStdString () == columnName)
		{
			return i;
		}
	}
	
	CSSMError::ThrowCSSMError (CSSMERR_DL_INVALID_FIELD_NAME);
}



int PartialRelation::GetColumnNumber (uint32 columnID)
{
	int i;
	for (i = 0; i < mNumberOfColumns; ++i)
	{
		if (mColumnIDs[i] == columnID)
		{
			return i;
		}
	}

	CSSMError::ThrowCSSMError (CSSMERR_DL_INVALID_FIELD_NAME);
}



