/*******************************************************************************
 *
 * Copyright (c) {2003-2019} Murex S.A.S. and its affiliates.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the MIT License
 * which accompanies this distribution, and is available at
 * https://opensource.org/licenses/MIT
 *
 *******************************************************************************/

#include <report/ReportGenerator.h>
#include <storage/Repository.h>
#include <purchase/Invoice.h>
#include <purchase/PurchasedBook.h>
#include <finance/CurrencyConverter.h>

using namespace purchase;

double getRoundedValueOf(double value) {
	return (static_cast<int>(value * 100 + 0.5)) / 100.0;
}

namespace report
{

	ReportGenerator::ReportGenerator() : repository_(main_repository::configuredRepository())
	{
	}

	double ReportGenerator::getTotalAmount() const
	{
		const auto invoiceMap = repository_->getInvoiceMap();
		double totalAmount = 0.0;
		for (const auto id2Invoice : invoiceMap)
		{
//		    BUG: There was a bug with the below line of code 
//			totalAmount += id2Invoice.second->computeTotalAmount();

//		    FIX: The above bug was fixed by the below 2 lines of code  
			const auto& invoice = *id2Invoice.second;
			totalAmount += finance::toUSD(invoice.computeTotalAmount(), invoice.getCountry().getCurrency());
		}
		
		return getRoundedValueOf(totalAmount);
	}

	int ReportGenerator::getTotalSoldBooks() const
	{
		const auto invoiceMap = repository_->getInvoiceMap();
		int totalSoldBooks = 0;
		for (const auto id2Invoice : invoiceMap)
		{
			for (const auto purchasedBook : id2Invoice.second->getPurchasedBooks())
			{
				totalSoldBooks += purchasedBook->getQuantity();
			}
		}
		return totalSoldBooks;
	}

	size_t ReportGenerator::getNumberOfIssuedInvoices() const
	{
		const auto invoiceMap = repository_->getInvoiceMap();
		return invoiceMap.size();
	}
}