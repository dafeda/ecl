#include <stdlib.h>
#include <stdbool.h>

#include <ert/util/test_work_area.hpp>
#include <ert/util/test_util.hpp>
#include <ert/util/time_t_vector.hpp>
#include <ert/util/util.h>

#include <ert/ecl/ecl_util.hpp>

void test_filename_report_nr() {
    test_assert_int_equal(
        78, ecl_util_filename_report_nr("Path/with/mixedCASE/case.x0078"));
    test_assert_int_equal(78, ecl_util_filename_report_nr("Case.X0078"));
    test_assert_int_equal(
        ECL_EGRID_FILE,
        ecl_util_get_file_type("path/WITH/xase/MyGrid.EGrid", NULL, NULL));
}

void test_filename_case() {
    char *f1 =
        ecl_util_alloc_filename(NULL, "mixedBase", ECL_EGRID_FILE, false, -1);
    char *f2 =
        ecl_util_alloc_filename(NULL, "UPPER", ECL_EGRID_FILE, false, -1);
    char *f3 =
        ecl_util_alloc_filename(NULL, "lower", ECL_EGRID_FILE, false, -1);
    char *f4 =
        ecl_util_alloc_filename(NULL, "lower1", ECL_EGRID_FILE, false, -1);
    char *f5 =
        ecl_util_alloc_filename(NULL, "UPPER1", ECL_EGRID_FILE, false, -1);

    test_assert_string_equal(f1, "mixedBase.EGRID");
    test_assert_string_equal(f2, "UPPER.EGRID");
    test_assert_string_equal(f3, "lower.egrid");
    test_assert_string_equal(f4, "lower1.egrid");
    test_assert_string_equal(f5, "UPPER1.EGRID");

    free(f1);
    free(f2);
    free(f3);
    free(f4);
    free(f5);
}

void test_file_list() {
    ecl::util::TestArea ta("file_list");
    stringlist_type *s = stringlist_alloc_new();

    for (int i = 0; i < 10; i += 2) {
        char *fname =
            ecl_util_alloc_filename(NULL, "case", ECL_RESTART_FILE, true, i);
        FILE *stream = util_fopen(fname, "w");
        fclose(stream);
        free(fname);
    }

    for (int i = 0; i < 10; i += 2) {
        char *fname = util_alloc_sprintf("Case.F%04d", i);
        FILE *stream = util_fopen(fname, "w");
        fclose(stream);
        free(fname);
    }

    for (int i = 0; i < 10; i += 2) {
        char *fname = util_alloc_sprintf("CaseMiXed.F%04d", i);
        FILE *stream = util_fopen(fname, "w");
        fclose(stream);
        free(fname);
    }

    ecl_util_select_filelist(NULL, "case", ECL_RESTART_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 5);
    for (int i = 0; i < 5; i++) {
        char *fname = ecl_util_alloc_filename(NULL, "case", ECL_RESTART_FILE,
                                              true, 2 * i);
        test_assert_string_equal(fname, stringlist_iget(s, i));
        free(fname);
    }

    ecl_util_select_filelist(NULL, "CaseMiXed", ECL_RESTART_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 5);

    util_make_path("path");
    for (int i = 0; i < 10; i++) {
        char *summary_file1 = ecl_util_alloc_filename(
            "path", "CASE1", ECL_SUMMARY_FILE, false, i);
        char *summary_file2 =
            ecl_util_alloc_filename("path", "CASE2", ECL_SUMMARY_FILE, true, i);
        char *restart_file1 = ecl_util_alloc_filename(
            "path", "CASE1", ECL_RESTART_FILE, false, i);

        FILE *f1 = fopen(summary_file1, "w");
        fclose(f1);

        FILE *f2 = fopen(summary_file2, "w");
        fclose(f2);

        FILE *f3 = fopen(restart_file1, "w");
        fclose(f3);

        free(summary_file1);
        free(summary_file2);
        free(restart_file1);
    }
    printf(
        "-----------------------------------------------------------------\n");
    ecl_util_select_filelist(NULL, "path/CASE1", ECL_SUMMARY_FILE, false, s);
    test_assert_int_equal(stringlist_get_size(s), 10);

    ecl_util_select_filelist(NULL, "path/CASE1", ECL_SUMMARY_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 0);

    ecl_util_select_filelist(NULL, "path/CASE2", ECL_SUMMARY_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 10);

    ecl_util_select_filelist("path", "CASE1", ECL_SUMMARY_FILE, false, s);
    test_assert_int_equal(stringlist_get_size(s), 10);

    ecl_util_select_filelist("path", "CASE1", ECL_SUMMARY_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 0);

    ecl_util_select_filelist("path", "CASE2", ECL_SUMMARY_FILE, true, s);
    test_assert_int_equal(stringlist_get_size(s), 10);

    stringlist_free(s);
}

int main(int argc, char **argv) {
    test_filename_report_nr();
    test_filename_case();
    test_file_list();
}
