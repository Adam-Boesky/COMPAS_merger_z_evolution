Creating a Docker image
=======================


GitHub CI/CD
------------
 
``Docker`` images of all versions of COMPAS (``dev`` branch) are available at the 
`TeamCOMPAS dockerHub page <https://hub.docker.com/u/teamcompas>`__. Building of these images is performed automatically by the 
``GitHub`` CI/CD process.

Whenever a push to `TeamCOMPAS/dev <https://github.com/TeamCOMPAS/COMPAS/tree/dev>`__ occurs, a continuous deployment process 
automatically
builds\ [#f1]_ a new image and deploys it to ``dockerHub`` with a `tag`\ [#f2]_ that corresponds to the value of ``VERSION_STRING`` 
in ``changelog.h`` (see :doc:`../changelog` for detailed information regarding ``changelog.h``).

At time of writing, `GitHub Actions`\ [#f3]_ facilitates the above process. While this is convenient (because it's free and well 
supported) it is somewhat slow - the COMPAS ``Docker`` image is available 5 - 10 minutes after pushing/merging. A future improvement 
may be to create a `runner`\ [#f4]_ locally with a high core count that can be used to compile COMPAS quickly.

The Github Actions configuration is in ``/.github/workflows/dockerhub-ci.yml``.

See the `Atlassian CI/CD <https://www.atlassian.com/continuous-delivery/principles/continuous-integration-vs-delivery-vs-deployment>`__ 
documentation for detailed information regarding the ``GitHub`` CI/CD process.


Dockerfile
----------

The Dockerfile\ [#f5]_ defines how the docker image is constructed.

Images are created as a combination of layers. During the build process, each layer is cached and only updated on subsequent builds 
if that layer would change. 

The Dockerfile for COMPAS is made up of 8 layers:

    **FROM ubuntu:18.04**\ [#f6]_ |br|
    Use `Ubuntu 18.04 <https://hub.docker.com/_/ubuntu>`__ as a base (provided by Docker Hub)
    
    **WORKDIR /app/COMPAS**\ [#f7]_ |br|
    Effectively ``cd /app/COMPAS`` within the container.
    
    **RUN apt-get update && apt-get install -y ...**\ [#f8]_ |br|
    Install the required dependencies.

        - `-y` so there's no prompt to install any of the packages.
        - `update` and `install` are in the same layer because now if there are any updates, it will force all of the dependencies
          to be re-installed

    **RUN pip3 install numpy**\ [#f8]_ |br|
    Install numpy.

    **COPY src/ src**\ [#f9]_ |br|
    Copy ``./src/`` directory from the local machine to ``./src`` in the container (remembering that `WORKDIR` changes the cwd).

    **RUN mkdir obj bin logs**\ [#f8]_ |br|
    Create the directories required by COMPAS.

    **ENV COMPAS_ROOT_DIR /app/COMPAS**\ [#f10]_ |br|
    Set the required environment variable(s).

    **RUN cd src && make -f Makefile.docker -j $(nproc)**\ [#f8]_ |br|
    Change to the ``src`` directory; make COMPAS using a specific makefile (see below) and as many cores as possible.

A Dockerfile usually ends with a ``CMD`` directive that specifies what command should run when the container is started\ [#f11]_. 
The COMPAS Dockerfile doesn't have a ``CMD`` directive because some users will want to run the executable directly and some will 
want to use ``runSubmit.py``.


Makefile.docker
---------------

A separate makefile is required for ``Docker`` in this scenario for two reasons:

    #. To separate compiled files from source files
    #. To prevent the usage of the ``gcc`` ``-march=native`` compiler option

``-march=native`` is a very useful optimisation for users who compile and run COMPAS on the same machine, however it causes fatal 
errors when running COMPAS on a machine for which it was not compiled. The ``-march=native`` compiler option selects the CPU for
which code should be generated by determining the processor type of the `compiling machine`. Using ``-march=native`` enables all
instruction subsets supported by the compiling machine, thus producing an executable file that will perform better than it would
if the full native instruction set was not available, but some of those instructions may not be available for use on machines of
different architectures - hence the possible fatal run-time errors if the code is run on machines with different architectures).
See `gcc x86-Options <https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html>`__ for detailed information regarding ``-march``.

The Docker makefile provided (``Makefile.docker``) functions exactly like the local makefile provided (``Makefile``) in all other
respects.  See :doc:`./COMPAS-local-build` for a detailed description of the local makefile functionality.


.. rubric:: Footnotes

.. [#f1] https://docs.docker.com/engine/reference/commandline/build/
.. [#f2] https://docs.docker.com/engine/reference/commandline/tag/
.. [#f3] https://github.com/features/actions
.. [#f4] https://help.github.com/en/actions/getting-started-with-github-actions/core-concepts-for-github-actions#runner
.. [#f5] https://docs.docker.com/engine/reference/builder/
.. [#f6] https://docs.docker.com/engine/reference/builder/#from
.. [#f7] https://docs.docker.com/engine/reference/builder/#workdir
.. [#f8] https://docs.docker.com/engine/reference/builder/#run
.. [#f9] https://docs.docker.com/engine/reference/builder/#copy
.. [#f10] https://docs.docker.com/engine/reference/builder/#env
.. [#f11] https://docs.docker.com/engine/reference/builder/#cmd

