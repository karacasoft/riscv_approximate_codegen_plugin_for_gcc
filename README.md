## Description

TODO

## Build

Make sure you have your riscv gcc cross compiler ready. In the `config.sh` script,
you may want to add the path of your compiler to your PATH variable.

Run:
```
bash build.sh
```

OR if you have the compiler in your PATH variable

```
make
```

## Usage

```
riscv32-unknown-linux-gnu-gcc -fplugin=/path/to/plugin.so &lt;input_files&gt; -o &lt;output_file&gt;
```

In your source code, you should add annotations to places you want to calculate approximately.
There are currently two ways to accomplish this.

### Pragmas

Syntax:
```
#pragma APPROX (xadd|xsub|xmul) (start|end)
```

Using these pragmas, you may mark the regions of your code that you want to be approximated.

These pragmas are tracked locally for each function. They cannot be declared outside of a function.

Example:
```
int f() {
	int a = 10;
	int b = a + 5;
#pragma APPROX xadd start
	int c = b + 10;      // <-- This will be approximated
	int d = c * a;
#pragma APPROX xmul start
	int e = b - a;
	int f = a + b + c;   // <-- All of these will be approximated
#pragma APPROX xadd end
	int g = a * c;       // <-- This will be approximated
#pragma APPROX xmul end
	return b;
}

int main() {
#pragma APPROX xsub start
	int b = f();         // <-- This will NOT be approximated. The compiler will issue a warning for this
	int a = 10 - b;      // <-- This will be approximated
#pragma APPROX xsub end
	int c;
	int d = a + b;
	int e = f();
	return 0;
}
```
### Attributes

Syntax:
```
__attribute__((approx))
```

When given to a function, this attribute will make all calculations inside the function approximate. When a function is marked with this attribute, the approximation pragmas provided by this plugin will be ignored.

Example:
```
int f() __attribute__((approx));

int f() {    // <-- Everthing inside this function will be approximated
	int a = 10;
	int b = a + 5;
	int c = b + 10;
	int d = c * a;
	int g = a * c;
	return b;
}

int main() {
	int b = f();
	int a = 10 - b;
	int c;
	int d = a + b;
	int e = f();

	return 0;
}

```