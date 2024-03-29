// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "gtest/gtest.h"
#include "seal/publickey.h"
#include "seal/context.h"
#include "seal/modulus.h"
#include "seal/keygenerator.h"

using namespace seal;
using namespace std;

namespace SEALTest
{
    TEST(PublicKeyTest, SaveLoadPublicKey)
    {
        stringstream stream;
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(1 << 6);
            parms.set_coeff_modulus(CoeffModulus::Create(64, { 60 }));

            auto context = SEALContext::Create(parms, false, sec_level_type::none);
            KeyGenerator keygen(context);

            PublicKey pk = keygen.public_key();
            ASSERT_TRUE(pk.parms_id() == context->key_parms_id());
            pk.save(stream);

            PublicKey pk2;
            pk2.load(context, stream);

            ASSERT_EQ(pk.data().uint64_count(), pk2.data().uint64_count());
            for (size_t i = 0; i < pk.data().uint64_count(); i++)
            {
                ASSERT_EQ(pk.data().data()[i], pk2.data().data()[i]);
            }
            ASSERT_TRUE(pk.parms_id() == pk2.parms_id());
        }
        {
            EncryptionParameters parms(scheme_type::BFV);
            parms.set_poly_modulus_degree(256);
            parms.set_plain_modulus(1 << 20);
            parms.set_coeff_modulus(CoeffModulus::Create(256, { 30, 40 }));

            auto context = SEALContext::Create(parms, false, sec_level_type::none);
            KeyGenerator keygen(context);

            PublicKey pk = keygen.public_key();
            ASSERT_TRUE(pk.parms_id() == context->key_parms_id());
            pk.save(stream);

            PublicKey pk2;
            pk2.load(context, stream);

            ASSERT_EQ(pk.data().uint64_count(), pk2.data().uint64_count());
            for (size_t i = 0; i < pk.data().uint64_count(); i++)
            {
                ASSERT_EQ(pk.data().data()[i], pk2.data().data()[i]);
            }
            ASSERT_TRUE(pk.parms_id() == pk2.parms_id());
        }
    }
}