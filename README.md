## Follow these steps to set up our repository on your computer

---

** 1. Download and install git if you haven't already**

You can install the github gui app but you will need a github account:
https://desktop.github.com/

Otherwise download and install command-line git, but you won't have the github GUI:
https://git-scm.com/download/win

** 2. Clone the repository **

At the top of the [repository page](https://bitbucket.org/hamsando/solardeg/src/master), click the "Clone" button.

In the github app, go to File -> Clone Repository...

In the URL field, enter the address you got from the clone button. Make sure to exclude the "git clone" part and only enter the address.

Enter a path where you want your local repository to be and click Clone.

If you are asked for credentials, log in with your Atlassian account

** 3. Add our CCS workspace to Code Composer Studio **

Open Code Composer Studio. Switch the workspace to the CCS_Workspace directory in your local repo.

If you don't see the projects you expect to see, go to File -> Import...

Choose Code Composer Studio -> CCS Projects

In the "Select search-directory:" field, click Browse... and choose the CCS_Workspace directory from the repo.

From there you should see the discovered projects. Click Finish.

---