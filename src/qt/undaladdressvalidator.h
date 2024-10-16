// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UNDAL_QT_UNDALADDRESSVALIDATOR_H
#define UNDAL_QT_UNDALADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class UndalAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit UndalAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Undal address widget validator, checks for a valid undal address.
 */
class UndalAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit UndalAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // UNDAL_QT_UNDALADDRESSVALIDATOR_H
