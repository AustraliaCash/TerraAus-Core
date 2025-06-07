// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2018-2024 The tAustraliaCash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Yahoo7 News 12 November 2018 How a police officer helped deliver baby on major highway";
    const CScript genesisOutputScript = CScript() << ParseHex("040174720fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        // Blocks 0 - 685,000 are conventional difficulty calculation, block reward and block times.
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xaa43989047f144331fc6400859c691b11c0e111ead1977511d340860c1c5ad1f");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 0.5 * 24 * 60 * 60; // 0.5 days - pre digishield
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minutes
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 100;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.fSimplifiedRewards = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1547078400; // January 10, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1576195200; // December 13, 2019
        //consensus.vDeployments[Consensus::DEPLOYMENT_CSV].min_activation_height = 48385;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1547078400; // January 10, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1576195200; // December 13, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000001335614ebeee1212");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x3762e27c84ef721dda2f6cf1b5fc1c2b5bbbf5a38037674e3af87bfd15eb3b7f");

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2000;
        consensus.fStrictChainId = false;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 675000;

        // Blocks 680000 - 690000 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 680000;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        digishieldConsensus.fPowAllowMinDifficultyBlocks = true;
        digishieldConsensus.nPowTargetTimespan = 2 * 60; // post-digishield: 2 min
        digishieldConsensus.nPowTargetSpacing = 30;
        digishieldConsensus.nCoinbaseMaturity = 80;

        // Blocks 690000+ are AuxPoW
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 690000;
        auxpowConsensus.fStrictChainId = false;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf1;
        pchMessageStart[1] = 0xc6;
        pchMessageStart[2] = 0xf2;
        pchMessageStart[3] = 0xcb;
        nDefaultPort = 1986;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1542015250, 2084598007, 0x1e0ffff0, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0xaa43989047f144331fc6400859c691b11c0e111ead1977511d340860c1c5ad1f"));
        assert(genesis.hashMerkleRoot == uint256S("0x273a20070d7cb57e4c77354d8bd5e01c1cf7d96ad32d91a7b7d183e30da12ef5"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed.taustraliacash.org", true));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed2.taustraliacash.org"));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed3.taustraliacash.org"));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed4.taustraliacash.org"));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed5.taustraliacash.org"));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed6.taustraliacash.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        // base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,23);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x07)(0x77)(0xC3)(0x1D).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x66)(0xAA)(0xF2).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (      0, uint256S("0xaa43989047f144331fc6400859c691b11c0e111ead1977511d340860c1c5ad1f"))
            (   2100, uint256S("0x0019f21b64cdae0379225a935dfc0f44030f45411eff5a2910b2f391875d872d"))
            (   4957, uint256S("0x7577ff0728fe984476bf2f9fa9261133d78da4f2af6feb036bf7e2a82a2d6055"))
            (  10212, uint256S("0xe004ad1378f39ca06bf0cff572a1c5f7369bb61add1d039f23c082cae9b3c2ea"))
            (  15215, uint256S("0x040dbc53aa7aeedef70600430446c7a636a3bf7a143b2644cf745c3bd9269deb"))
            (  16915, uint256S("0x1bd4305f93404ddcc65da27e2898494df8f8d04d55c8bb2ff8b7f0636d2fcc73"))
            (  18648, uint256S("0x1a193a850f29228ddee8cae435c51cc215e962446b7d0166222e497056cbdd76"))
            (  20135, uint256S("0xff5f29ec67713b790b8e72089c58724a6f7ed7d10bac441e7a03a756cd3c41ae"))
            (  22356, uint256S("0xc1c6d7e497a5a63723045baa144bd8ca167beded471170a5ecc3494d6400107c"))
            (  24872, uint256S("0xd93d562dc901fa171a7bb98120ebc3ac225640bf26143cb2cb851aef43b99341"))
            (  29517, uint256S("0x89158c1bb3dde6e01f30f476922ec8ebae195dbb2063ccef46a33eeec2923b15"))
            (  30210, uint256S("0x34e2d49d3138bb73429efbcd87303fca223b60f1f9b1e19d840579d4944b8e3a"))
            (  31502, uint256S("0xd5bc86ab92257f0e3eb149f69ba180410092c6465639d8fd5ab8525c1a2a0200"))
            (  32985, uint256S("0x58815a2fdec8b91cf0e84197cb0eea88dd705edd962e4784a43d6783fe03963f"))
            (  37846, uint256S("0x1e7d1f67de29e32e06c23426f9346a99876a24d664fcddbdd2705e8054dc5530"))
            (  40000, uint256S("0xf2243cce020a8354758e4f26d4398656e80b2b91add91681d8a729248ff3e473"))
            (  45000, uint256S("0x49f2754bc381ac77c1cc36210e8862935de2ff5346a66b9817906bb9223c0c90"))
            (  50000, uint256S("0xfb09f0e9fc709c22e84b014bfbe11eb1a8ede673ab0873f03bf1bc70662b2a62"))
            (  55000, uint256S("0x370f787f4daf4ac52c9d2560298de8e25efc40930804f55fac91f88cca91fecc"))
            (  60000, uint256S("0x7ce107de1168a9052fb4bee934e3acf8a9694f6e5310b74fe40ffcd4536ddf1f"))
            (  65000, uint256S("0x4e500fd331c28b3dccc714978d437b4cdde7675f69b84423c176f58b177a668b"))
            (  70000, uint256S("0x2a954526a229c4679a950b08754460bd62ea7d88f7634fa020cf49c4bfe21312"))
            (  75000, uint256S("0xddcc79c889ae5a72aab32abe826478b865e0df6cc4e0608bea0c3195237d0745"))
            (  80000, uint256S("0x9b6230d9066906640bcfe26bc92b3eb1f1f52ea59c7aa107844dd7902ce1f70c"))
            (  85000, uint256S("0xd410c1187292bb4e1a17e88a499fbd727cf1742b2eab2eb3257135e4522e6a77"))
            (  90000, uint256S("0x8d4c267b390daa0913397f4357dd6b38f119f1d463bfcdcb1d5c4457eec6c2ec"))
            (  95000, uint256S("0x16ae3b52e588a4750d1670127a66b428b64396709824e6b08d8050369b932c54"))
            ( 130000, uint256S("0x233f0791e8beab0ea34af5aa4db4f9e9e78b4e6f570e7af3a26a94432019d697"))
            ( 232059, uint256S("0x1b340cd2dd8990b4e8c1c686038cb4d882cd6a71991bb0a0381027af7851e892"))
            ( 333333, uint256S("0x5dc49c1298629a9b2e5915791b45aaca420bf52115fa0e6d66555c93fb3f1d4d"))
            ( 444444, uint256S("0xb4b7cabc8d571489148dbb4ec3c330688aa76c5cfd0d893c9f0da5c645bf3861"))
            ( 555555, uint256S("0xdb53c32f3c6068f48a239ab4e55c4c3923cf716450e14a4f25b0cf108cad7972"))
            ( 633333, uint256S("0x3762e27c84ef721dda2f6cf1b5fc1c2b5bbbf5a38037674e3af87bfd15eb3b7f"))
        };

        chainTxData = ChainTxData{
            // Data as of block  (height ).
            // Tx estimate based on average between 2021-07-01 (3793538) and 2022-07-01 (4288126)
            1715123921, // * UNIX timestamp of last checkpoint block
            963324,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.040207        // * estimated number of transactions per second after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        consensus.nHeightEffective = 0;
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nSubsidyHalvingInterval = 10000;
        consensus.nMajorityEnforceBlockUpgrade = 5;
        consensus.nMajorityRejectBlockOutdated = 7;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x00");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2880; // 2 days (note this is significantly lower than Bitcoin standard)
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // Not active
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 0; // Not active

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // 1547078400;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // 1947078400;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0; // 1547078400;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // 1947078400;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

// AuxPoW parameters
        consensus.nAuxpowChainId = 0x2000;
        consensus.fStrictChainId = false;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 0;

        // Blocks 680000 - 685000 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 168;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        digishieldConsensus.nPowTargetTimespan = 60 * 2; // post-digishield: 2 min
        digishieldConsensus.nPowTargetSpacing = 30;
        digishieldConsensus.nCoinbaseMaturity = 20;

        // Blocks 690000+ are AuxPoW
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 179;
        auxpowConsensus.fStrictChainId = false;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 2018;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541015250, 319977, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x32fbb5ecde32b9506eec7e75efd9d390f7e9d3dc06aa7a9a430f0a131307be7e"));
        assert(genesis.hashMerkleRoot == uint256S("0x273a20070d7cb57e4c77354d8bd5e01c1cf7d96ad32d91a7b7d183e30da12ef5"));

        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "seed.taustraliacash.org", true));
        vSeeds.push_back(CDNSSeedData("taustraliacash.org", "testseed.taustraliacash.org", true));
        vSeeds.push_back(CDNSSeedData("triplezen.org", "testseed.triplezen.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,113); // 0x71
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x32fbb5ecde32b9506eec7e75efd9d390f7e9d3dc06aa7a9a430f0a131307be7e"))
        };

        chainTxData = ChainTxData{
            // Data as of block af23c3e750bb4f2ce091235f006e7e4e2af453d4c866282e7870471dcfeb4382 (height 3976284)
            1657647467, // * UNIX timestamp of last checkpoint block
            5353803,    // * total number of transactions between genesis and last checkpoint
            0.02        // * estimated number of transactions per second after that timestamp
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // Faster than normal for regtest (2,520 instead of 10,080)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2000; // Jack Test
        consensus.fStrictChainId = false;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 68;

        // Blocks 900000 - 920000 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 68;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        digishieldConsensus.nPowTargetTimespan = 2; // post-digishield: 2 sec for regtest
        digishieldConsensus.nPowTargetSpacing = 30;
        digishieldConsensus.nCoinbaseMaturity = 24;

        // Blocks 920000+ are AuxPoW
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 68;
        auxpowConsensus.fStrictChainId = false;
        auxpowConsensus.fAllowLegacyBlocks = true;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541015250, 319977, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        assert(consensus.hashGenesisBlock == uint256S("0x32fbb5ecde32b9506eec7e75efd9d390f7e9d3dc06aa7a9a430f0a131307be7e"));
        assert(genesis.hashMerkleRoot == uint256S("0x273a20070d7cb57e4c77354d8bd5e01c1cf7d96ad32d91a7b7d183e30da12ef5"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("32fbb5ecde32b9506eec7e75efd9d390f7e9d3dc06aa7a9a430f0a131307be7e"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);  // 0x6f
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);  // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);  // 0xef
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
