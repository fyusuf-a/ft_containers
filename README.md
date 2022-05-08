# ft_containers

`ft_containers` is a project aimed at recreating part of the C++ standard library: the `vector`, `map` and `stack` templates of the STL.
My implementation of `map` uses AVL trees for performance.
More info [here](https://en.cppreference.com/w/cpp/container). Ressources about AVL trees can be found on [wikipedia](https://en.wikipedia.org/wiki/AVL_tree) and [here](https://runestone.academy/ns/books/published/pythonds/Trees/AVLTreeImplementation.html).

## Building

```
make
```

This will produce tests executable located in subfolder `./tests`. A version compiled with this project's headers will be generated as well as one with the standard library headers. This is to measure time differences between the two.

## Usage

```
make test
```

This will launch executables built in the preceding section. Note that it will also launch tests from the standard C++ library gathered by [Caceresenzo](https://github.com/caceresenzo/).
