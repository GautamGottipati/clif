/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// C++ declarations for clif backend tests.  Not necessarily Google
// style compliant nor ever actually compiled into object code.

#ifndef CLIF_BACKEND_TEST_H_
#define CLIF_BACKEND_TEST_H_

#include <stdint.h>
// These are all declared in the same file to make lookup inside the
// TU complicated enough to be convincing.

// Declarations for testing TranslationUnitAST.

int simple;
const int sample = 0;

void Func();

void PolymorphicFunc(int x);
void PolymorphicFunc(char *x);

class aClass;

class aClass {
 public:
  static constexpr char kStringConst[] = "abcdefg";
  static constexpr char kAnotherStringConst[] = {'a', 'b'};
  int x;
  const int constant_int = 1;
  static void StaticMember();
 private:
  int y;
 protected:
  int z;
};

typedef aClass TypedeffedClass;

class aFinalClass final : public aClass {
  void Foo(aClass a);
};


enum Enum {
  E1 = 321,
  E2 = 432,
  E3 = 543
};

enum Flag {
  F1 = 1,
  F2 = 2,
  F3 = 4,
  F5 = 8
};

struct Arg {
  Enum e;
};

class IntArg {
 public:
  int a;
  operator int() {
    return a;
  }
};

class Class {
 public:
  int MethodWithDefaultArg(Arg arg = { EnumValue });
  int MethodWithDefaultIntArg(IntArg arg = (IntArg){ 100 });
  int MethodWithDefaultNullptr(const Arg* arg = nullptr);
  int MethodWithDefaultFlag(int flag = F1 | F2);
  bool MethodWithDefaultBoolArgWithSideEffects(bool b = BoolFunc() || true);
  bool MethodWithDefaultBoolArgWithoutSideEffects(bool b = false);
  static bool BoolFunc();
  void Func();
  void MemberA();
  void PolymorphicFunc(int x);
 private:
  static const Enum EnumValue;
};

const Enum Class::EnumValue = E2;

class AbstractClass {
 public:
  virtual void Func() = 0;
};


class DerivedClass : public Class {
 public:
  DerivedClass() { }
  int MemberB(int x);
  void PolymorphicFunc(void *y);
};

class DerivedClass2 : public Class { };

void BaseFunctionValue(Class c);
void BaseFunctionPtr(Class* c);
void BaseFunctionRef(const Class& c);

namespace Namespace {

int simple;

void Func();

class Class {
 public:
  void Func();
  void PolymorphicFunc(int x);
  void PolymorphicFunc(void *y);
};

class bClass {
 public:
  int x;
};

enum class anEnum { a, b, c, d };
enum anotherEnum { e, f, g, h};

typedef int typedeffed_int;

// Class to test using clauses outside the namespace.
class UsingClass {
 public:
  int x;
  using anEnumHiddenInAUsingDeclaration = anEnum;
};

}  // namespace Namespace

namespace Globally {
namespace Qualified {
class ForwardDecl;
}
void FunctionWithPartiallyQualifiedDecl(Qualified::ForwardDecl* d);
};
void FuncGloballyQualifiedNamePtrParam(
    const ::Globally::Qualified::ForwardDecl* c);

namespace some {
int int_id(int x);

}  // namespace some

//
using Namespace::UsingClass;

// Declarations for testing ClifMatcher

// tests for return type
void FuncReturnsVoid();
int FuncReturnsInt() noexcept;
int64_t FuncReturnsInt64();
float FuncReturnsFloat();
void VoidFuncIntPointerParam(int *x);
int FuncIntPointerParam(int *x);
int FuncIntRefParam(int &x); // NOLINT
int FuncReturnsTwoInts(int *x);
const int* FuncReturnsConstIntPtr();

// tests for parameter counts
void FuncOneParam(int x);
void FuncTwoParams(int x, int y);
void FuncOneReqOneOptParams(int x, int y = 0);
int FuncOneReqOneOptParamsReturnsInt(int x, int y = 0);
int FuncTwoParamsTwoReturns(int x, int y, int* z);

// More parameter checking
void VoidFuncConstIntRefParam(const int &x);
void VoidFuncNamespaceParam(const Namespace::bClass b);

void FuncConstVsNonConst(const int&x, const int& y);
void FuncConstVsNonConst(const int&x, int& y);  // NOLINT

// Be sure that we describe the presence of a default ctor accurately.
class ClassWithDefaultCtor {
 public:
  ClassWithDefaultCtor() { }
  void MethodConstVsNonConst() const;
  void MethodConstVsNonConst();
};
void VoidFuncClassParamWithDefaultCtor(ClassWithDefaultCtor c);
class ClassWithoutDefaultCtor {
 public:
  ClassWithoutDefaultCtor(int x) { } // NOLINT
};
void VoidFuncClassParamWithoutDefaultCtor(ClassWithoutDefaultCtor c);
void VoidFuncClassParamWithDefaultCtor(ClassWithDefaultCtor c);
class ClassWithPrivateDefaultCtor {
 private:
  ClassWithPrivateDefaultCtor(int x) { } // NOLINT
};
void VoidFuncClassParamWithPrivateDefaultCtor(ClassWithPrivateDefaultCtor c);
class ClassWithDeletedCopyCtor {
 public:
  ClassWithDeletedCopyCtor(const ClassWithDeletedCopyCtor&) = delete;
};

class ClassMovableButUncopyable {
 public:
  // Delete copy-constructor and copy assignment operator
  ClassMovableButUncopyable(const ClassMovableButUncopyable&) = delete;
  ClassMovableButUncopyable& operator=(const ClassMovableButUncopyable&) =
      delete;

  // Ensure move-constructor and move assignment operator exist
  ClassMovableButUncopyable(ClassMovableButUncopyable&&) = default;
  ClassMovableButUncopyable& operator=(ClassMovableButUncopyable&&) = default;

  ClassMovableButUncopyable Factory();
};


class ClassPureVirtual {
 public:
  virtual void SomeFunction() = 0;
};

void FuncAbstractParam(const ClassPureVirtual* x);
void FuncAbstractParam(const AbstractClass& x);
void FuncForwardDeclared(const Globally::Qualified::ForwardDecl* x);

// tests to be sure we don't match a return in const param.
void FuncConstIntPointerParam(const int *x);
void FuncConstIntRefParam(const int &x);

class AnotherClass { public: void Foo(); };

template<typename T> class SpecializationsHaveConstructors {
public:
  SpecializationsHaveConstructors(T t);
};

template<class T>
class ComposedType : public AnotherClass{
 public:
  T t;
  ComposedType(int x);
  T FunctionWithTemplatedReturnType();
  void FunctionWithTemplatedParameter(const T t);
};

typedef ComposedType<int> TypedeffedTemplate;

void FuncTemplateParam(ComposedType<int> x);
void FuncTemplateParamLValue(const ComposedType<int>& x);
template<typename A> void SimpleFunctionTemplateA(A x);
template<typename A, typename B> void UndeducableTemplate(A x);
template<typename A> void PointerArgTemplate(A* x);
ComposedType<ComposedType<int>> Clone(const ComposedType<ComposedType<int>>);

namespace GrandParents {

class greatgrandparent {};
class grandparent : public virtual greatgrandparent {};
}  // namespace GrandParents

using GrandParents::greatgrandparent;
using GrandParents::grandparent;

class parent : public grandparent {};
class child : public parent {};

class grandfather {};
class grandmother {};
class multiparent : public grandfather, public grandmother {};
class multichild : public multiparent {};

// Smart pointers are tested by clif/testing/python:all,
// because this file doesn't have the full C++ compilation
// environment, and we want to avoid that dependency here anyway.
// std::unique_ptr<child> unique_ptr_child;

// Output parameter before input parameter.
void UnwrappableFunction(child* y, int z);

class UncopyableClass {
  explicit UncopyableClass(int x);
  UncopyableClass(const UncopyableClass& other) = delete;
 private:
};

void FuncTakesUncopyableClass(UncopyableClass uc);
void FuncTakesUncopyableClassConstRef(const UncopyableClass& uc);

class PrivateDestructorClass {
 private:
  ~PrivateDestructorClass() {}
};

class NoCopyAssign {
  int a_;
  NoCopyAssign& operator=(const NoCopyAssign&) = delete;
 public:
  explicit NoCopyAssign(int a = 0) : a_(a) {}
  int A() { return a_; }
};

unsigned long long UnsignedLongLongReturn();  // NOLINT[runtime/int]
void TakesBool(bool x);
void TakesInt(int x);
void TakesFloat(float x);
void TakesPtr(int* x);

// Global operator overload.
bool operator==(const grandmother& gm, const grandfather& gp);

// Local operator overload
class OperatorClass {
 public:
  bool operator==(const OperatorClass&);
  bool operator==(const int&);
};

// Overload to be found by global lookup even though Clif defines it
// in a class. (cpp_opfunction)

bool operator!=(const OperatorClass&, const OperatorClass&);

// Hack because we don't have a complete compilation environment here.
namespace std {
template< class > class function;
template< class R, class... Args > class function<R(Args...)>;

template< typename T > class unique_ptr;
}

class DynamicBase {
 public:
  virtual ~DynamicBase();
};

class DynamicDerived : public DynamicBase {};

void FuncUniqPtrToBuiltinTypeArg(std::unique_ptr<long long int>);  // NOLINT [runtime/int]
std::unique_ptr<long long int> FuncUniqPtrToBuiltinTypeReturn();  // NOLINT [runtime/int]
void FuncWithUniqPtrToDynamicBaseArg(std::unique_ptr<DynamicBase>);

class OpaqueClass;
typedef OpaqueClass* MyOpaqueClass;
void FuncWithPtrOutputArg(MyOpaqueClass* opaque);

template <typename T>
void TemplateFuncWithOutputArg1(T* t);
template <typename T>
float TemplateFuncWithOutputArg2(T* t);
template <typename T>
void TemplateFuncWithOutputArg3(const T& t, int* i);
template <typename T>
float TemplateFuncWithOutputArg4(const T& t, int* i);
template <typename T>
T TemplateFuncWithOutputArg5(const T& t, int* i);

class ClassWithQualMethodsAndParams {
 public:
  void Method1(const int a);

  void Method2(const Class& s);

  Class Method3();

  void Method4(const Class& s) const;

  void Method5(const int a, Class* s) const;
};

class ClassWithDeprecatedMethod {
 public:
  void MethodWithDeprecatedOverload(Class& c);  // NOLINT

  __attribute__((deprecated("A deprecated method")))
  void MethodWithDeprecatedOverload(Class* c);

  __attribute__((deprecated("A deprecated method")))
  void DeprecatedMethod(Class* c);

  __attribute__((deprecated("A deprecated method")))
  void DeprecatedMethodWithDeprecatedOverload(Class* c);

  __attribute__((deprecated("A deprecated method")))
  void DeprecatedMethodWithDeprecatedOverload(Class& c);  // NOLINT
};

void OverloadedFunction(int x);
void OverloadedFunction(std::function<void(child)> x);

void FunctionWithDeprecaredOverload(Class& c);  // NOLINT

__attribute__((deprecated("A deprecated function")))
void FunctionWithDeprecatedOverload(Class* c);

__attribute__((deprecated("A deprecated function")))
void DeprecatedFunction(Class* c);

void FunctionToPtrConversion(grandmother g0,
                             grandmother& g1,  // NOLINT
                             grandmother* g2,
                             grandmother** g3);

class ClassWithNonDefaultConstructor {
 public:
  explicit ClassWithNonDefaultConstructor(int i);

 protected:
  void Method();
};

class ClassWithInheritedConstructor : public ClassWithNonDefaultConstructor {
 public:
  using ClassWithNonDefaultConstructor::ClassWithNonDefaultConstructor;
  using ClassWithNonDefaultConstructor::Method;
};

class OuterClass1 {
 public:
  class InnerClass {
   public:
    int a;
  };
};

class OuterClass2 {
 public:
  class InnerClass {
   public:
    int b;
  };
};

template <typename... T>
class VariadicTemplateClass {};

void FuncWithVariadicTemplateClassInput(
    const VariadicTemplateClass<int, int, int>& v);
VariadicTemplateClass<int, int, int> FuncWithVariadicTemplateClassReturn();

// Preprocessor directive to test inclusions from another file.
#line 100 "another_file.h"

namespace clif {
// Conversion function matcher ignores type of first argument.
void PyObjAs(int x, int **y);
void PyObjAs(int x, grandfather **y);
}  // namespace clif

namespace a_user {
namespace defined_namespace {
void Clif_PyObjAs(int x, bool **y);
void Clif_PyObjAs(int x, grandmother **y);
void Clif_PyObjAs(int x, std::unique_ptr<int>* g);
void Clif_PyObjAs(int x, std::unique_ptr<grandmother>* g);

bool operator==(const Class& x, int);

}  // namespace defined_namespace
}  // namespace a_user


template<typename T> class ClassTemplateInAnotherFile {
 public:
  T* SomeFunction(T t);
};

namespace const_ref_tests {

class ClassB { };

class ClassA {
 public:
  ClassA(const ClassB& other) { }
};

void PassByValue(ClassA ls);
void PassByConstRef(const ClassA& ls);
void PassByRef(ClassA& ls);  //NOLINT [runtime/reference]
}  // namespace const_ref_tests


// Preprocessor directive to get back to the original file.
#line 300 "test.h"

typedef ClassTemplateInAnotherFile<int> ClassTemplateDeclaredInImportedFile;
ClassTemplateDeclaredInImportedFile f;
#endif  // CLIF_BACKEND_TEST_H_