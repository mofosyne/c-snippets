# c-snippets

Various self contained C function snippets not ready or useful enough to be part of a util repo, but is useful enough to keep in mind.

To improve maintainability and discoverbility. Each snippet in each file is runnable via this tip https://stackoverflow.com/questions/2482348/run-c-or-c-file-as-a-script

```
//usr/bin/clang "$0" && exec ./a.out "$@"
int main(){
    return 0;
}
```

This is also handy as we could have each snippet be self testing eventally as well.
