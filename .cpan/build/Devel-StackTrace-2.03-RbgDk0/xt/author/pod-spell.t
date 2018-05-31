use strict;
use warnings;
use Test::More;

# generated by Dist::Zilla::Plugin::Test::PodSpelling 2.007004
use Test::Spelling 0.12;
use Pod::Wordlist;


add_stopwords(<DATA>);
all_pod_files_spelling_ok( qw( bin lib ) );
__DATA__
Bessarabov
CPAN
Cantrell
DROLSKY
DROLSKY's
Dagfinn
Dave
David
Devel
Fowler
Frame
Graham
Ilmari
Ivan
Knop
Mannsåker
Mark
PayPal
Ricardo
Rolsky
Rolsky's
Signes
StackTrace
autarch
david
drolsky
haarg
ilmari
ivan
lib
mark
rjbs
stacktrace
