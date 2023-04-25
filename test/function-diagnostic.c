// RUN:  %{c2wit_clang_cc1} -fsyntax-only -verify %s


__attribute__((annotate("wit-export")))
__attribute__((annotate("wit-name"))) // expected-error{{naming annotate attribute only accept 1 argument exactly}}
int foo(int a, float b, double c);

__attribute__((annotate("wit-export")))
__attribute__((annotate("wit-name", "one", "more", "argument"))) // expected-error{{naming annotate attribute only accept 1 argument exactly}}
int bar(int, float, double);


__attribute__((annotate("wit-name"))) // expected-error{{naming annotate attribute only accept 1 argument exactly}}
int baz(int, float, double);


__attribute__((annotate("wit-name", "one", "more", "argument"))) // expected-error{{naming annotate attribute only accept 1 argument exactly}}
int bazz(int, float, double);
