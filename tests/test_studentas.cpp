#include <gtest/gtest.h>
#include "studentas.h"
#include <sstream>
#include <utility>

static Studentas sukurti() {
    Studentas s;
    s.vardas = "Jonas"; s.pavarde = "Jonaitis";
    s.n = 3; s.nd = {8,9,7}; s.egzaminas = 10;
    return s;
}

TEST(Studentas, DefaultKonstruktorius) {
    Studentas s;
    EXPECT_EQ(s.vardas, "");
    EXPECT_EQ(s.n, 0);
    EXPECT_EQ(s.egzaminas, 0);
    EXPECT_TRUE(s.nd.empty());
}

TEST(Studentas, KopijavimoKonstruktorius) {
    Studentas s1 = sukurti();
    Studentas s2(s1);
    EXPECT_EQ(s2.vardas, "Jonas");
    s2.vardas = "Petras";
    EXPECT_EQ(s1.vardas, "Jonas");
}

TEST(Studentas, KopijavimoPriskyrimas) { /* ... */ }
TEST(Studentas, PerkelimoKonstruktorius) { /* tikrina kad s1.n == 0 po move */ }
TEST(Studentas, PerkelimoPriskyrimas) { /* ... */ }

TEST(Studentas, OperatorIsvedimas) {
    Studentas s = sukurti();
    std::ostringstream oss; oss << s;
    EXPECT_EQ(oss.str(), "Jonas Jonaitis 3 8 9 7 10");
}

TEST(Studentas, OperatorIvedimas) {
    std::istringstream iss("Ona Kazlauskiene 2 6 8 9");
    Studentas s; iss >> s;
    EXPECT_EQ(s.vardas, "Ona");
    EXPECT_EQ(s.n, 2);
    EXPECT_EQ(s.egzaminas, 9);
}