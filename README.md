# Mars Pathplanning - Solution
The program solves the problem by computing **permutations** of the points that have to be reached by ARDITO.

For each permutation, the total distance is then computed by summing the euclidean distances between consecutive points. The first point and the last point are to be considered as consecutive (as requested):

(https://github.com/team-diana/DIANA-Recruitment-2022-23/tree/Mars_Path#software-requirements)

> [...] the shortest possible path that starts at a given point, reaches all of the requested coordinates and ends back to the starting point. [...]

## Algorithm for permutations
The algorithms is recursive and has a complexity of **O(n!)**.

At the termination condition, the function checks wether the newly found path is shorter than any other paths previously found, and keeps track of the shortest path accordingly.


