# fast-shell
# Description
<p>FastShell is a project submission for <strong>CEN-232 Operating Systems @ Burch University.</strong> It is developed by Amar Genjac & Amar Fazlić. FastShell covers some basic functions of a shell together with some custom functions. 
Functions that are implemented are: </p> 
<ul>
  <strong>
   <li>cd
     <li>help
      <li>clear
        <li>exit
          <li>cowsay
            <li>rm
              <li>cat
                <li>frokbomb
                  <li>color
</ul></strong>

# Installation
<p>There are 3 files inside the src folder.  To run FastShell you need to navigate using terminal to the source folder and compile files using command:<strong> gcc -o outputname main.c prompt.c </strong>
Output name is the name you want to use to run the program. To run program simply run <strong>./outputname</strong> in the terminal.</p>


![running](https://user-images.githubusercontent.com/76213933/166460951-c1397c70-daeb-41bb-9b68-718faa899379.png)



# Usage
When FastShell is opened, it is possible to run the <strong>help</strong> command to see all possible functions implemented. Functions that are receiving arguments also have <strong>h</strong> argument that they can receive to see help for the selected function (e.g <strong>rm h</strong>). Functions <strong>clear</strong> and <strong>exit</strong> do not contain <strong>h</strong> command since they are straightforward and they don't have other options.


![rmex](https://user-images.githubusercontent.com/76213933/166460936-a8f0ea10-7bd2-4c48-be3b-d4d2e5054ee4.png)



Different functions contain different arguments that can be explored through <strong>h</strong> command.
## Writing to file
Functions that have possible outputs to the console, also have possible outputs to .txt file when argument <strong>o</strong> is passed.

![cow](https://user-images.githubusercontent.com/76213933/166468008-a8c6260b-435d-439f-9cdd-e2ce52e2503c.png)

