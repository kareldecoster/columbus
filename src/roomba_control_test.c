#include <check.h>
#include <roomba_control.h>

START_TEST(test_roomba_create)
{
    roomba_t* roomba;

    roomba = roomba_create();
    ck_assert(roomba != NULL);
    roomba_destroy(&roomba);
    ck_assert(roomba==NULL);    
}
END_TEST

Suite * roomba_control_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("roomba_control");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_roomba_create);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = roomba_control_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

