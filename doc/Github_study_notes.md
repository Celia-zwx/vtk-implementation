# Github Notes:

#### 1. Local folder long time no update

##### Issue：

```
PS D:\vtk-implementation\test> git push origin main
fatal: 'origin' does not appear to be a git repository
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.
```

##### Solution：

```
git remote add origin https://github.com/Celia-zwx/vtk-implementation.git
git push origin main
```



#### 2. Issue: commit and lose files

- ##### Issue: lose track of files during push or pull process, sometimes happens at switching the branch

- ##### Reason

- ##### Solution

  1. **Check Previous Commits**: Use `git log` to view the commit history. Look for commits where the files were last present. You can inspect the changes in each commit to see when and how the files were removed.

  2. **Check Out Previous Commit**:

     Once you've identified the commit where the files were last present, you can check out that commit to restore the files.

     ```
     git checkout <commit_hash> -- <path/to/lost_file>
     ```

     Replace `<commit_hash>` with the commit hash where the file was last present and `<path/to/lost_file>` with the path to the lost file.

  3. Once you've identified the commit where the files were last present, you can check out that commit to restore the files.

     ```
     git checkout <commit_hash> -- <path/to/lost_file>
     ```

     Replace `<commit_hash>` with the commit hash where the file was last present and `<path/to/lost_file>` with the path to the lost file.

  4. 

     If you are on a different branch master, you would like to push the change to branch main

     ```
     git checkout main
     git merge master
     git push origin main
     ```

  5.  warning: in the working copy of 'ha', LF will be replaced by CRLF the next time Git touches it

     ```
     git config --global core.autocrlf true
     
     ```

#### 3. Change folder name

- ##### Issue: if you change folder name locally, and push the change directly to the branch, you will receive warning:

```
warning: adding embedded git repository: doc
hint: You've added another git repository inside your current repository.
hint: Clones of the outer repository will not contain the contents of
hint: the embedded repository and will not know how to obtain it.
hint: If you meant to add a submodule, use:
hint:
hint:   git submodule add <url> doc
hint:
hint: If you added this path by mistake, you can remove it from the
hint: index with:
hint:
hint:   git rm --cached doc
hint:
hint: See "git help submodule" for more information.
```

- ###### Reason

  1. This warning indicates that you've added another nested Git repository within your current Git repository, which might have been unintentional. This can often lead to issues because clones of the outer repository won't include the contents of the nested repository, nor will they know how to obtain it.

     If you intended to add a submodule instead of a nested Git repository, you can follow the suggestion to use the `git submodule add` command to add the submodule.

     If you added this nested repository by mistake and don't need it, you can remove it from the index using the `git rm --cached` command. However, note that this command won't delete the actual files from the disk. 

- ###### Solution

```
 git rm --cached doc
 git add .
 git commit -m "remove cache"
 git push origin main
 
```

