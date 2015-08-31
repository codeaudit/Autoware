/*
 *  Copyright (c) 2015, Nagoya University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Autoware nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "precomp.hpp"
#include "_lsvm_routine.h"

int allocFilterObject(CvLSVMFilterObject **obj, const int sizeX,
                      const int sizeY, const int numFeatures)
{
    (*obj) = (CvLSVMFilterObject *)malloc(sizeof(CvLSVMFilterObject));
    (*obj)->sizeX           = sizeX;
    (*obj)->sizeY           = sizeY;
    (*obj)->numFeatures     = numFeatures;
    (*obj)->fineFunction[0] = 0.0f;
    (*obj)->fineFunction[1] = 0.0f;
    (*obj)->fineFunction[2] = 0.0f;
    (*obj)->fineFunction[3] = 0.0f;
    (*obj)->V.x         = 0;
    (*obj)->V.y         = 0;
    (*obj)->V.l         = 0;
    (*obj)->H = (float *) malloc(sizeof (float) *
                                (sizeX * sizeY  * numFeatures));
    for(int i = 0; i < sizeX * sizeY * numFeatures; i++)
    {
        (*obj)->H[i] = 0.0f;
    }
    return LATENT_SVM_OK;
}

int freeFilterObject (CvLSVMFilterObject **obj)
{
    if(*obj == nullptr) return LATENT_SVM_MEM_NULL;
    free((*obj)->H);
    free(*obj);
    (*obj) = nullptr;
    return LATENT_SVM_OK;
}

int allocFeatureMapObject(CvLSVMFeatureMap **obj, const int sizeX,
                          const int sizeY, const int numFeatures)
{
    (*obj) = (CvLSVMFeatureMap *)malloc(sizeof(CvLSVMFeatureMap));
    (*obj)->sizeX       = sizeX;
    (*obj)->sizeY       = sizeY;
    (*obj)->numFeatures = numFeatures;
    (*obj)->map = (float *) malloc(sizeof (float) *
                                  (sizeX * sizeY  * numFeatures));
    for(int i = 0; i < sizeX * sizeY * numFeatures; i++)
    {
        (*obj)->map[i] = 0.0f;
    }
    return LATENT_SVM_OK;
}
int freeFeatureMapObject (CvLSVMFeatureMap **obj)
{
    if(*obj == nullptr) return LATENT_SVM_MEM_NULL;
    free((*obj)->map);
    free(*obj);
    (*obj) = nullptr;
    return LATENT_SVM_OK;
}

int allocFeaturePyramidObject(CvLSVMFeaturePyramid **obj,
                              const int numLevels)
{
    (*obj) = (CvLSVMFeaturePyramid *)malloc(sizeof(CvLSVMFeaturePyramid));
    (*obj)->numLevels = numLevels;
    (*obj)->pyramid    = (CvLSVMFeatureMap **)malloc(
                         sizeof(CvLSVMFeatureMap *) * numLevels);
    return LATENT_SVM_OK;
}

int freeFeaturePyramidObject (CvLSVMFeaturePyramid **obj)
{
    if(*obj == nullptr) return LATENT_SVM_MEM_NULL;
    for(int i = 0; i < (*obj)->numLevels; i++)
    {
        freeFeatureMapObject(&((*obj)->pyramid[i]));
    }
    free((*obj)->pyramid);
    free(*obj);
    (*obj) = nullptr;
    return LATENT_SVM_OK;
}

int allocFFTImage(CvLSVMFftImage **image, int numFeatures, int dimX, int dimY)
{
    *image = (CvLSVMFftImage *)malloc(sizeof(CvLSVMFftImage));
    (*image)->numFeatures = numFeatures;
    (*image)->dimX         = dimX;
    (*image)->dimY         = dimY;
    (*image)->channels     = (float **)malloc(sizeof(float *) * numFeatures);
    int size = 2 * dimX * dimY;
    for (int i = 0; i < numFeatures; i++)
    {
        (*image)->channels[i] = (float *)malloc(sizeof(float) * size);
        for (int j = 0; j < size; j++)
        {
            (*image)->channels[i][j] = 0.0f;
        }
    }
    return LATENT_SVM_OK;
}

int freeFFTImage(CvLSVMFftImage **image)
{
    if (*image == nullptr) return LATENT_SVM_OK;
    for (int i = 0; i < (*image)->numFeatures; i++)
    {
        free((*image)->channels[i]);
        (*image)->channels[i] = nullptr;
    }
    free((*image)->channels);
    (*image)->channels = nullptr;
    return LATENT_SVM_OK;
}