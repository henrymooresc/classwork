/* 
The following is a course scheduling tool in Prolog.
The first part defines a database of courses with their pre-requisites
and co-requisites taken from the current Computer Engineering undergraduate
curriculum.  It does not include courses picked from a list such as
humanities, STS, and tedchnical electives, only the required courses.
Next comes a section of predicats that defines which of these courses have
already been taken and the egrade assigned.  We aren't going to use the
grade but you can see how we might expand it.  I only have a couple of
these entered, you can use them, remove them, or add more if you like to
test different situations.
After that are rules.  Near the bottom the main user interface is the
check_course and check_schedule predicates that check to see if all the
requirements are satisfied.  The first takes a single course name, and
the latter takes a list of courses.  Both return a list of missing 
pre-requisites and a list of missing co-requisites.  In the case of
check_schedule if a ico-requisite course is listed in the schedule it
should not appear as missing.  If a course has already been taken, the
code prints a simple error message and returns fail.  The other predicates
represent a stepwise implementation of these two, and simple utilities
that let you explore the relationships in the database.  They are fairly
easy so you should do them first, and then use them to implement the more
complex ones.  The last predicate is suggest_course that tries to pick a
course that has not been taken and has all of its requisites completed.
You are free to add other predicates you feel you need in order to
implement the required ones.  There is more than one way to solve the
problems.  You will also want to look at built-in predicates either via
the documentation or the apropos and help predicates in SWIPL.  In
particular this predicate may be useful:

append(List1,List2,List+List2).
*/

/* course( <course-name>, <pre-req list>, <co-req list> ). */

course(ch1010,[],[]).
course(cpsc1110,[],[]).
course(ece2010,[],[]).
course(ece2020,[math1080],[phys2210]).
course(ece2090,[],[]).
course(ece2110,[],[ece2020]).
course(ece2120,[ece2110],[ece2620]).
course(ece2220,[cpsc1110],[]).
course(ece2230,[ece2220],[]).
course(ece2620,[ece2020,phys2210,math2060],[]).
course(ece2720,[ece2010,cpsc1110],[]).
course(ece2730,[],[ece2720]).
course(ece3110,[ece2120],[]).
course(ece3170,[ece2620,math2080],[ece3300]).
course(ece3220,[ece2720,ece2230],[]).
course(ece3270,[ece3710],[]).
course(ece3300,[ece2620,math2080],[]).
course(ece3520,[ece2230],[math4190]).
course(ece3710,[ece2720],[ece2620]).
course(ece3720,[],[ece3710]).
course(ece4090,[ece3300],[]).
course(ece4950,[ece3710,ece2230,ece3200],[ece4090]).
course(ece4960,[ece3270,ece3520,ece4950,ece4090],[]).
course(engl1030,[],[]).
course(engr1020,[],[]).
course(engr1410,[engr1020],[]).
course(math1060,[],[]).
course(math1080,[math1060],[]).
course(math2060,[math1080],[]).
course(math2080,[math2060],[]).
course(math3110,[math1080],[]).
course(math4190,[math3110],[]).
course(phys1120,[],[math1060]).
course(phys2210,[],[math1080]).

/* completed( <course-name>, <grade> ) */

completed(C) :- completed(C,_).

completed(engr1020,a).
completed(ch1010,b).
completed(math1060,a).
completed(engl1030,b).

/* RULES */

/* returns true if all courses are completed */
/*complete_all(List) :- check_completed(List).*/

complete_all([]).

complete_all([H|T]) :-
    completed(H),
    complete_all(T).

/* return courses in Req-List not completed */
/*missing_req(Rlist, Mlist) :- 
    check_missing(Rlist, Mlist).*/

missing_req([], []).

missing_req([Hi|Ti], [Hi|To]) :-
    \+ completed(Hi),
    missing_req(Ti, To).

missing_req([_|Ti], To) :- 
    missing_req(Ti, To).

/* return true if all pre-reqs satisified for Course */
prereq_satisfied(Course) :-
    course(Course, Plist, _),
    complete_all(Plist).

/* return a list of missing pre-reqs for Course */
prereq_missing(Course,NP) :-
    course(Course, Plist, _),
    missing_req(Plist, NP).

/* return true if all co-reqs satisified for Course */
coreq_satisfied(Course) :-
    course(Course, _, Clist),
    complete_all(Clist).

/* return a list of missing co-reqs for Course */
coreq_missing(Course,NC) :-
    course(Course, _, Clist),
    missing_req(Clist, NC).

/* return lists of missing pre-reqs in L1 and co-reqs in L2 for course C */
/* prints a message if course is already completed */
check_course(C,L1,L2) :-
    completed(C),
    write('Course already completed'), nl,
    !, fail;
    
    prereq_missing(C, L1),
    coreq_missing(C, L2).

/* returns all of the missing pre-reqs in L1 and co-reqs in L2 for all */
/* courses in LC  or returns true if there are none missing */
check_schedule(LC,L1,L2) :-
    /*recur_schedule(LC,L1,Clist),*/
    recur_prereqs(LC,L1),
    recur_coreqs(LC,Clist),
    check_enrolled(LC,Clist,L2).

recur_prereqs([], _).
recur_prereqs([H|T], L) :-
    \+ prereq_satisfied(H),
    prereq_missing(H, Plist),
    recur_prereqs(T, NewL),
    append(NewL, Plist, L).

recur_prereqs([_|T], L) :-
    recur_prereqs(T, L).

recur_coreqs([], _).
recur_coreqs([H|T], L) :-
    \+ coreq_satisfied(H),
    coreq_missing(H, Clist),
    recur_coreqs(T, NewL),
    append(NewL, Clist, L).

recur_coreqs([_|T], L) :-
    recur_coreqs(T, L).

/*
recur_schedule([],_,_).

recur_schedule([HLC|TLC],L1,L2) :-
    prereq_satisfied(HLC),
    coreq_satisfied(HLC),
    recur_schedule(TLC,L1,L2);

    prereq_missing(HLC,Plist),
    coreq_missing(HLC,Clist),
    
    recur_schedule(TLC,NewL1,NewL2),

    append(NewL1, Plist, L1),
    append(NewL2, Clist, L2).
*/

check_enrolled(_,[],_).
check_enrolled(LC, [H|T], L2) :-
    \+ member(H, LC),
    check_enrolled(LC, T, NewL2),
    append(NewL2, [H], L2).

check_enrolled(LC, [_|T], L2) :-
    check_enrolled(LC, T, L2).

/* prints one (or more) courses that are not completed and do have their */
/* requisites satisfied. 
suggest_course(C).*/
