#include <stdlib.h>
#include <stdbool.h>

#include <ert/util/bool_vector.hpp>
#include <ert/util/test_util.hpp>
#include <ert/util/util.h>

#include <ert/ecl/ecl_kw.hpp>

void test_int() {
    int N = 1000;
    int i;
    ecl_kw_type *kw = ecl_kw_alloc("KW", N, ECL_INT);
    for (i = 0; i < N; i++)
        test_assert_int_equal(0, ecl_kw_iget_int(kw, i));

    ecl_kw_free(kw);
}

void test_double() {
    int N = 1000;
    double i;
    ecl_kw_type *kw = ecl_kw_alloc("KW", N, ECL_DOUBLE);
    for (i = 0; i < N; i++)
        test_assert_double_equal(0, ecl_kw_iget_double(kw, i));

    ecl_kw_free(kw);
}

void test_float() {
    int N = 1000;
    int i;
    ecl_kw_type *kw = ecl_kw_alloc("KW", N, ECL_FLOAT);
    for (i = 0; i < N; i++)
        test_assert_int_equal(0, ecl_kw_iget_float(kw, i));

    ecl_kw_free(kw);
}

void test_bool() {
    int N = 100;
    bool *data = (bool *)util_malloc(N * sizeof *data);
    ecl_kw_type *kw = ecl_kw_alloc("BOOL", N, ECL_BOOL);
    for (int i = 0; i < N / 2; i++) {
        ecl_kw_iset_bool(kw, 2 * i, true);
        ecl_kw_iset_bool(kw, 2 * i + 1, false);

        data[2 * i] = true;
        data[2 * i + 1] = false;
    }

    const bool *internal_data = ecl_kw_get_bool_ptr(kw);

    test_assert_int_equal(memcmp(internal_data, data, N * sizeof *data), 0);
    ecl_kw_free(kw);
    free(data);
}

int main(int argc, char **argv) {
    test_int();
    test_double();
    test_float();
    test_bool();
    exit(0);
}
