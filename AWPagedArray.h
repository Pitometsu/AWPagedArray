//
// AWPagedArray.h
//
// Copyright (c) 2014 Alek Åström
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#import <Foundation/Foundation.h>

@protocol AWPagedArrayDelegate;

/**
 * This class acts as a proxy for NSArray, when data is loaded in batches, called "pages".
 * @discussion The recommendation is to use this class in conjunction with a data controller class which populates the paged
 *array with pages of data, while casting the paged array back as an NSArray to its owner.
 *
 * This class is inspired by NSFetchRequest's batching mechanism which returns a custom NSArray subclass.
 * @see NSFetchRequest fetchBatchSize
 */
@interface AWPagedArray <__covariant ObjectType> : NSProxy

/**
 * The designated initializer for this class
 * Note that the parameters are part of immutable state
 */
- (instancetype)initWithCount:(NSUInteger)count
               objectsPerPage:(NSUInteger)objectsPerPage
             initialPageIndex:(NSInteger)initialPageIndex;

/**
 * Convenience initializer with initialPageIndex = 1
 */
- (instancetype)initWithCount:(NSUInteger)count objectsPerPage:(NSUInteger)objectsPerPage;

/**
 * Sets objects for a specific page in the array
 * @param objects The objects in the page
 * @param page The page which these objects should be set for, pages start with index 1
 * @throws AWPagedArrayObjectsPerPageMismatchException when page size mismatch the initialized objectsPerPage property
 * for any page other than the last.
 */
- (void)setObjects:(NSArray<ObjectType> *)objects forPage:(NSUInteger)page;

- (NSUInteger)pageForIndex:(NSUInteger)index;
- (NSIndexSet *)indexSetForPage:(NSUInteger)page;

@property (nonatomic, readwrite) NSUInteger totalCount;
@property (nonatomic, readonly) NSUInteger objectsPerPage;
@property (nonatomic, readonly) NSUInteger numberOfPages;
@property (nonatomic, readonly) NSInteger initialPageIndex;

/**
 * Contains NSArray instances of pages, backing the data
 */
@property (nonatomic, readonly) NSDictionary<NSNumber *, NSArray<ObjectType> *> *pages;

@property (nonatomic, weak) id<AWPagedArrayDelegate> delegate;

- (void)enumerateExistingObjectsUsingBlock:(void (^)(ObjectType obj, NSUInteger idx, BOOL *stop))block;
- (NSArray<ObjectType> *)existingObjects;
- (void)invalidateContents;

- (ObjectType)rawObjectAtIndex:(NSUInteger)index;

- (NSArray<ObjectType> *)toArray;

@end

@protocol AWPagedArrayDelegate<NSObject>

/**
 * Called when the an object is accessed by index
 *
 * @param pagedArray the paged array being accessed
 * @param index the index in the paged array
 * @param returnObject an id pointer to the object which will be returned to the receiver of the accessor being called.
 *
 * @discussion This delegate method is only called when the paged array is accessed by the objectAtIndex: method or by
 *subscripting.
 * The returnObject pointer can be changed in order to change which object will be returned.
 */
- (void)pagedArray:(AWPagedArray *)pagedArray willAccessIndex:(NSUInteger)index returnObject:(__autoreleasing id *)returnObject;

@end
